#include "IPOInput.h"

IPOInput::IPOInput(wxFrame* frame,wxWindowID id,wxPoint p,wxString name,wxString d,wxString defaultValue)
{
    m_name = new wxTextCtrl(frame,10000,name,p+wxPoint(0,0),wxSize(80,30));
    m_name->SetEditable(false);

    m_input = new wxTextCtrl(frame,id,defaultValue,p+wxPoint(81,0),wxSize(100,30));

    m_d = new wxTextCtrl(frame,10001,d,p+wxPoint(182,0),wxSize(40,30));
    m_d->SetEditable(false);
}

IPOInput::~IPOInput()
{
    //dtor
}

void IPOInput::SetModified(bool flag)
{
    m_input->SetEditable(flag);
}

double IPOInput::getValue()
{
    return str2int(m_input->GetLineText(0).ToStdString());
}

double IPOInput::str2int(const string &string_temp)
{
    double int_temp;
    stringstream stream(string_temp);
    stream>>int_temp;
    return int_temp;
}
