import React, { PureComponent } from "react";

import "./ConsumerForm.less";
import RegisterForm from "../../../../../../common/register-form/RegisterForm";

export default class ConsumerForm extends PureComponent {
  render() {
    const formSchema = {
      type: "object",
      required: ["name", "balance"],
      additionalProperties: false,
      properties: {
        name: {
          title: "Name",
          type: "string",
          minLength: 2,
          maxLength: 100,
          pattern: "^[a-zA-Z]+$"
        },
        balance: {
          title: "Balance",
          type: "string"
        }
      }
    };

    const uiSchema = {
      name: {
        "ui:placeholder": "John Doe"
      },
      balance: {
        // "ui:widget": "",
        "ui:placeholder": "0"
      }
    };

    return (
      <div className="consumer-form">
        <RegisterForm formSchema={formSchema} uiSchema={uiSchema} />
      </div>
    );
  }
}