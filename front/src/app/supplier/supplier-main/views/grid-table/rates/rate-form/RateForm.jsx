import React, { PureComponent } from "react";

import "./RateForm.less";
import RegisterForm from "../../../../../../common/register-form/RegisterForm";
import AppStore from "../../../../../../../store/AppStore";
import Eos from "../../../../../../../helpers/eos";

export default class RateForm extends PureComponent {
  constructor(props) {
    super(props);

    this.addRate = this.addRate.bind(this);
  }

  addRate(formData) {
    const { description, type, price } = formData;

    const dict = { electricity: "electricity", RFID: "billrfid" };

    Eos.sendTransaction("addrate", {
      description,
      billing_account: dict[type],
      meta: price
    })
      .then(result => {
        console.log(result);
        this.props.onCloseModal();
      })
      .catch(error => {
        console.error(error);
      });
  }

  render() {
    const formSchema = {
      type: "object",
      required: ["description", "type", "price"],
      additionalProperties: false,
      properties: {
        description: {
          title: "Description",
          type: "string"
        },
        type: { title: "Type", type: "string", enum: ["RFID", "electricity"] },
        price: { title: "Price", type: "string" }
      }
    };

    const uiSchema = {
      name: { "ui:placeholder": "John Doe" },
      price: { "ui:placeholder": "0" }
    };

    return (
      <div className="rate-form">
        <RegisterForm
          formSchema={formSchema}
          uiSchema={uiSchema}
          onSubmit={this.addRate}
          title={"Add rate"}
        />
      </div>
    );
  }
}
