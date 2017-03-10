/***************************************************************
 * Name:      IPOSystemMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2017-03-08
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef IPOSYSTEMMAIN_H
#define IPOSYSTEMMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "IPOSystemApp.h"
#include "include/IPOCanvs.h"
#include "include/IPOInput.h"
#include "include/IPODataShow.h"
#include "include/RobotInfo.h"

class IPOSystemFrame: public wxFrame
{
    public:
        IPOSystemFrame(wxFrame *frame, const wxString& title);
        ~IPOSystemFrame();
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            idButtonStartOnClick,
            idButtonPauseOnClick,
            idButtonResumeOnClick,
            idButtonResetOnClick,
            idCanvsP,
            idCanvsS,
            idInputCurPosition,
            idInputDesPosition,
            idInputSpeed,
            idInputPro,
            idInputAcsN,
            idInputDecN,
            idDataShow,
            idButtonSaveOnClick
        };
        enum status{
            STATUS_STABLE = 0,
            STATUS_ACC,
            STATUS_DEC,
            STATUS_STOP,
            STATUS_END

        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClickButtonStart(wxCommandEvent& event);
        void OnClickButtonPause(wxCommandEvent& event);
        void OnClickButtonResume(wxCommandEvent& event);
        void OnClickButtonReset(wxCommandEvent& event);
        void OnClickButtonSave(wxCommandEvent& event);
        void OnTimer(wxTimerEvent& event);
        DECLARE_EVENT_TABLE()

        wxButton * buttonStart;
        wxButton * buttonPause;
        wxButton * buttonResume;
        wxButton * buttonReset;
        wxButton * buttonSave;
        IPOCanvs * canvs_p;
        IPOCanvs * canvs_s;
        IPOInput * inputCurPosition;
        IPOInput * inputDesPosition;
        IPOInput * inputSpeed;
        IPOInput * inputPro;
        IPOInput * inputAcsN;
        IPOInput * inputDecN;
        IPODataShow * dataShow;

        wxPoint lastPoint;
        wxTimer m_timer;
        enum status m_curStatus;

        RobotInfo m_robotInfo;

        void makeInputEnalbe(bool flag);
        void showDiaLog(wxString info);
        double abs(double value) { return value<0? -value:value;}

        bool m_startLessEnd;
        double adjustSpeedAndPos(int n);
};


#endif // IPOSYSTEMMAIN_H
