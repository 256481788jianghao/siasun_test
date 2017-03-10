/***************************************************************
 * Name:      IPOSystemApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2017-03-08
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "IPOSystemApp.h"
#include "IPOSystemMain.h"

IMPLEMENT_APP(IPOSystemApp);

bool IPOSystemApp::OnInit()
{
    IPOSystemFrame* frame = new IPOSystemFrame(0L, _("IPOSystem"));
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();


    return true;
}
