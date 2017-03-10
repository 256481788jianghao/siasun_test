#ifndef IPOINPUT_H
#define IPOINPUT_H

#include <wx/wx.h>
#include <sstream>
using namespace std;


class IPOInput
{
    public:
        IPOInput(wxFrame* frame,wxWindowID id,wxPoint p,wxString name,wxString d,wxString defaultValue = "0");
        virtual ~IPOInput();
        void SetModified(bool flag);
        double getValue();
    protected:
    private:
        wxTextCtrl* m_name;
        wxTextCtrl* m_input;
        wxTextCtrl* m_d;
        double str2int(const string &string_temp);

};

#endif // IPOINPUT_H
