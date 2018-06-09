//
// Created by quantum on 09.06.18.
//

#include "supplier.hpp"

void supplier::adduser(account_name user_account, std::string description, std::string meta) {
    require_auth( _self );

    auto itr = _users.find( user_account );
    eosio_assert(itr != _users.end(), "User already exists");

    _users.emplace( _self, [&]( auto& a ) {
        a.account = user_account;
        a.description = description;
        a.meta = meta;
    });

}

void supplier::addrate(string description, account_name billing_account, string meta) {
    require_auth( _self );

    //
    _rates.emplace( _self, [&]( auto& a ) {
        a.rate_id = _rates.available_primary_key();
        a.description = description;
        a.billing_account = billing_account;
        a.meta = meta;
    });

}

void supplier::adddevice(account_name device_account, account_name user_account, uint64_t rate_id,
                         string description) {
    require_auth( _self );

    auto itr = _devices.find( device_account );
    eosio_assert(itr != _devices.end(), "Device already exists");

    _devices.emplace( _self, [&]( auto& a ) {
        a.account = device_account;
        a.user_account = user_account;
        a.rate_id = rate_id;
        a.description = description;
    });
}

void supplier::addbalance(account_name user_account, asset quantity) {
    require_auth( _self );

    eosio_assert( quantity.symbol == token_symbol, "Wrong symbol" );

    auto itr = _users.find( user_account );
    eosio_assert(itr == _users.end(), "User doesn't exist");

    _users.modify( itr, 0, [&]( auto& a ) {
        a.balance += quantity;
        eosio_assert( a.balance >= quantity, "Overflow detected" );
    });

}

void supplier::subbalance(account_name user_account, asset quantity) {
    eosio_assert(false, "Not implemented");
}

void supplier::devicesignal(account_name device_account, uint64_t data) {
    require_auth( device_account );

    auto device_itr = _devices.find( device_account );
    eosio_assert(device_itr == _devices.end(), "Device doesn't registered");

    auto rate_itr = _rates.find( device_itr->rate_id );
    eosio_assert(rate_itr == _rates.end(), "Rate doesn't registered");

    auto user_itr = _users.find( device_itr->user_account );
    eosio_assert(user_itr == _users.end(), "User doesn't registered");

    eosio::action(
            permission_level{ _self, N(active) },
            rate_itr->billing_account, N(bill),
            std::make_tuple(data, device_itr->user_account, user_itr->meta, rate_itr->meta)
    ).send();

}

void supplier::dopayment(account_name from, asset quantity) {

}



EOSIO_ABI( supplier, (adduser)(addrate)(adddevice)(addbalance)(devicesignal)(dopayment) )
