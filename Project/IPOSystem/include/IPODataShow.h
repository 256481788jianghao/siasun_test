#ifndef IPODATASHOW_H
#define IPODATASHOW_H

#include <wx/wx.h>
#include <sstream>
using namespace std;

class IPODataShow : public wxTextCtrl
{
    public:
        IPODataShow(wxFrame* frame,wxWindowID id,wxPoint p);
        virtual ~IPODataShow();
        void appendData(wxInt32 id,double speed,double position);
    protected:
    private:
        wxString title;
        wxString data;
        wxString makeFormat(wxString id,wxString p,wxString v);
        string Int_to_String(double n);
};

#endif // IPODATASHOW_H
