//
// Created by Eenae on 09.06.2018.
//

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/currency.hpp>

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "billing.hpp"
#include "debug_tools.hpp"
#include "common.hpp"


using eosio::asset;
using eosio::const_mem_fun;
using eosio::indexed_by;
using std::string;
using std::istringstream;
using eosio::permission_level;
using common::token_symbol;


class billing_electricity : /*public billing,*/ public eosio::contract {
public:
    billing_electricity(account_name self) :
            contract(self) {}

    // @abi action
    void bill(
            account_name supplier_account,
            account_name device_account,
            uint64_t device_data,
            account_name user2bill,
            string user_meta,
            string billing_meta
    ) {
        // device_data is a number of measurements sent
        // billing_meta: <float: watts/hour per measurement>\t<uint: payment per kWt/hour>
        print_block_start("billing_electricity:bill", device_data, user2bill, user_meta, billing_meta);

        require_auth(device_account);
        //todo check permission to call this method


        float wattPerMeasurement;
        uint64_t paymentPerKWT;

        char * billing_meta_char = new char [billing_meta.length()+1];
        strcpy (billing_meta_char, billing_meta.c_str());

        sscanf(billing_meta_char, "%f\t%llu", &wattPerMeasurement, &paymentPerKWT);

        eosio::print( "wattPerMeasurement = ", wattPerMeasurement, "  paymentPerKWT = ", paymentPerKWT, "\n" );

        asset quantity = asset(wattPerMeasurement * paymentPerKWT / 1000, token_symbol);
        eosio::action(
                permission_level{ device_account, N(active) },
                supplier_account, N(dopayment),
                std::make_tuple(_self, device_account, user2bill, quantity)
        ).send();

        print_block_end("billing_electricity:bill", device_data, user2bill, user_meta, billing_meta);
    }
};


EOSIO_ABI( billing_electricity, (bill) )
