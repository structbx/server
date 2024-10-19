
#include "actions/forms.h"

using namespace StructBI;

Actions::Forms::Forms(FunctionData& function_data) : 
    read_a01_(function_data)
    ,read_specific_a01_(function_data)
    ,read_specific_a02_(function_data)
    ,add_a01_(function_data)
    ,add_a02_(function_data)
    ,add_a03_(function_data)
    ,modify_a01_(function_data)
    ,modify_a02_(function_data)
    ,modify_a03_(function_data)
    ,delete_a01_(function_data)
    ,delete_a02_(function_data)
{
    
}

Actions::Forms::ReadA01::ReadA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action->set_sql_code(
        "SELECT " \
            "f.* " \
        "FROM forms f " \
        "WHERE " \
            "id_space = ? "
    );
    action->AddParameter_("id_space", get_space_id(), false);
}

Actions::Forms::ReadSpecificA01::ReadSpecificA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms WHERE id = ? AND id_space = ?");

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

Actions::Forms::ReadSpecificA02::ReadSpecificA02(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::ReadSpecificA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms WHERE identifier = ? AND id_space = ?");

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);

}

Actions::Forms::AddA01::AddA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::AddA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador para este espacio ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

Actions::Forms::AddA02::AddA02(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::AddA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("INSERT INTO forms (identifier, name, state, privacity, description, id_space) VALUES (?, ?, ?, ?, ?, ?)");

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        auto string_param = param->get_value()->ToString_();
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(string_param == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(string_param.size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = IDChecker().Check_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action_->AddParameter_("state", "", true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("privacity", "", true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
}

Actions::Forms::AddA03::AddA03(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::AddA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, length, required, id_column_type, id_form) " \
        "SELECT " \
            "? " \
            ",? " \
            ",? " \
            ",? " \
            ",(SELECT id FROM forms_columns_types WHERE identifier = 'int-number') " \
            ",(SELECT id FROM forms WHERE identifier = ? and id_space = ?) "
    );

    action_->AddParameter_("identifier", "id", false);
    action_->AddParameter_("name", "ID", false);
    action_->AddParameter_("length", "11", false);
    action_->AddParameter_("required", 1, false);
    action_->AddParameter_("identifier", "", true);
    action_->AddParameter_("space_id", get_space_id(), false);
}

Actions::Forms::ModifyA01::ModifyA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier, id_space FROM forms WHERE id = ?");
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

Actions::Forms::ModifyA02::ModifyA02(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::ModifyA02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id != ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador en este espacio ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
}

Actions::Forms::ModifyA03::ModifyA03(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::ModifyA03::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE forms " \
        "SET identifier = ?, name = ?, state = ?, privacity = ?, description = ? " \
        "WHERE id = ? AND id_space = ?"
    );

    // Parameters and conditions
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = IDChecker().Check_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action_->AddParameter_("state", "", true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("privacity", "", true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);

}

Actions::Forms::Delete01::Delete01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::Delete01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier FROM forms WHERE id = ?");
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

Actions::Forms::Delete02::Delete02(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::Forms::Delete02::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("DELETE FROM forms WHERE id = ? AND id_space = ?");
    action_->AddParameter_("id", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
}