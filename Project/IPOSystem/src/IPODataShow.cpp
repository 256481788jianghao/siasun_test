#include "IPODataShow.h"
#include <iomanip>

string IPODataShow::Int_to_String(double n)
{

    ostringstream stream;
    stream<<setprecision(10)<<n; //nÎªintÀàÐÍ
    return stream.str();

}

IPODataShow::IPODataShow(wxFrame* frame,wxWindowID id,wxPoint p)
    :wxTextCtrl(frame,id,"",p,wxSize(350,520),wxTE_MULTILINE)
{
    title = makeFormat("index","position","speed");
    this->AppendText(title);
    this->SetEditable(false);
}

IPODataShow::~IPODataShow()
{
    //dtor
}

wxString IPODataShow::makeFormat(wxString id,wxString p,wxString v)
{
    return id+"            "+p+"            "+v+"\n";
}

void IPODataShow::appendData(wxInt32 id,double speed,double position)
{
    data = makeFormat(Int_to_String(id),Int_to_String(speed),Int_to_String(position));
    this->AppendText(data);
}
