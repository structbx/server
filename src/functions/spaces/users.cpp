
#include "functions/spaces/users.h"

using namespace StructBI::Functions::Spaces;

Users::Users(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
    ReadUserOutSpace_();
    /*Add_();
    Delete_();*/
}

void Users::Read_()
{
    // Function GET /api/spaces/users/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/users/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT nu.username, sp.created_at " \
        "FROM _naf_users nu " \
        "JOIN spaces_users sp ON sp.id_naf_user = nu.id " \
        "WHERE sp.id_space = ?"
    );
    action1->AddParameter_("id_space", get_space_id(), true)
    ->SetupCondition_("condition-id_space", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de espacio no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}

void Users::ReadUserOutSpace_()
{
    // Function GET /api/spaces/users/out/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/users/out/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT nu.username "
        "FROM _naf_users nu "
        "JOIN organizations_users ou ON ou.id_naf_user = nu.id "
        "LEFT JOIN spaces_users su ON "
            "su.id_naf_user = nu.id AND "
            "su.id_space = (SELECT s.id FROM spaces s JOIN spaces_users su2 ON su2.id_space = s.id WHERE identifier = ? AND su2.id_naf_user = ? LIMIT 1) "
        "WHERE "
            "ou.id_organization = (SELECT id_organization FROM organizations_users WHERE id_naf_user = ?) "
            "AND su.id_naf_user IS NULL "
    );
    action1->AddParameter_("identifier_space", "", true)
    ->SetupCondition_("condition-identifier_space", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
    action1->AddParameter_("id_user", get_id_user(), false);
    action1->AddParameter_("id_user2", get_id_user(), false);

    get_functions()->push_back(function);
}
