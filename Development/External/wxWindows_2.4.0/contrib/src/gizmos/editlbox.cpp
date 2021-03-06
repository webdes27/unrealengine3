/////////////////////////////////////////////////////////////////////////////
// Name:        editlbox.cpp
// Purpose:     ListBox with editable items
// Author:      Vaclav Slavik
// RCS-ID:      $Id: editlbox.cpp,v 1.13.2.1 2002/11/12 23:28:35 VS Exp $
// Copyright:   (c) Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "editlbox.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/gizmos/editlbox.h"
#include "wx/sizer.h"
#include "wx/listctrl.h"




// list control with auto-resizable column:
class CleverListCtrl : public wxListCtrl
{
public:
   CleverListCtrl(wxWindow *parent,
                  wxWindowID id = -1,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxLC_ICON,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString &name = _T("listctrl"))
         : wxListCtrl(parent, id, pos, size, style, validator, name)
    {
        CreateColumns();
    }

    void CreateColumns()
    {
        InsertColumn(0, _T("item"));
        SizeColumns();
    }

    void SizeColumns()
    {
         int w = GetSize().x;
#ifdef __WXMSW__
         w -= wxSystemSettings::GetMetric(wxSYS_VSCROLL_X) + 6;
#else
         w -= 2*wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
#endif
         SetColumnWidth(0, w);
    }

private:
    DECLARE_EVENT_TABLE()
    void OnSize(wxSizeEvent& event)
    {
        SizeColumns();
        event.Skip();
    }
};

BEGIN_EVENT_TABLE(CleverListCtrl, wxListCtrl)
   EVT_SIZE(CleverListCtrl::OnSize)
END_EVENT_TABLE()

#include "eldel.xpm"
#include "eldown.xpm"
#include "eledit.xpm"
#include "elnew.xpm"
#include "elup.xpm"

IMPLEMENT_CLASS(wxEditableListBox, wxPanel)

enum
{
    // ID value doesn't matter, it won't propagate out of wxEditableListBox
    // instance
    wxID_ELB_DELETE = wxID_HIGHEST + 1,
    wxID_ELB_NEW,
    wxID_ELB_UP,
    wxID_ELB_DOWN,
    wxID_ELB_EDIT,
    wxID_ELD_LISTCTRL
};

BEGIN_EVENT_TABLE(wxEditableListBox, wxPanel)
    EVT_LIST_ITEM_SELECTED(wxID_ELD_LISTCTRL, wxEditableListBox::OnItemSelected)
    EVT_LIST_END_LABEL_EDIT(wxID_ELD_LISTCTRL, wxEditableListBox::OnEndLabelEdit)
    EVT_BUTTON(wxID_ELB_NEW, wxEditableListBox::OnNewItem)
    EVT_BUTTON(wxID_ELB_UP, wxEditableListBox::OnUpItem)
    EVT_BUTTON(wxID_ELB_DOWN, wxEditableListBox::OnDownItem)
    EVT_BUTTON(wxID_ELB_EDIT, wxEditableListBox::OnEditItem)
    EVT_BUTTON(wxID_ELB_DELETE, wxEditableListBox::OnDelItem)
END_EVENT_TABLE()

wxEditableListBox::wxEditableListBox(wxWindow *parent, wxWindowID id,
                          const wxString& label,
                          const wxPoint& pos, const wxSize& size,
                          long style,
                          const wxString& name)
   : wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL, name)
{
    m_style = style;
    m_bEdit = m_bNew = m_bDel = m_bUp = m_bDown = NULL;    

    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel *subp = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize,
                                wxSUNKEN_BORDER | wxTAB_TRAVERSAL);
    wxSizer *subsizer = new wxBoxSizer(wxHORIZONTAL);
    subsizer->Add(new wxStaticText(subp, -1, label), 1, wxALIGN_CENTRE_VERTICAL | wxLEFT, 4);

#ifdef __WXMSW__
    #define BTN_BORDER 4
    // FIXME - why is this needed? There's some reason why sunken border is
    //         ignored by sizers in wxMSW but not in wxGTK that I can't
    //         figure out...
#else
    #define BTN_BORDER 0
#endif

    if ( m_style & wxEL_ALLOW_EDIT )
    {
        m_bEdit = new wxBitmapButton(subp, wxID_ELB_EDIT, wxBitmap(eledit_xpm));
        subsizer->Add(m_bEdit, 0, wxALIGN_CENTRE_VERTICAL | wxTOP | wxBOTTOM, BTN_BORDER);
    }

    if ( m_style & wxEL_ALLOW_NEW )
    {
        m_bNew = new wxBitmapButton(subp, wxID_ELB_NEW, wxBitmap(elnew_xpm));
        subsizer->Add(m_bNew, 0, wxALIGN_CENTRE_VERTICAL | wxTOP | wxBOTTOM, BTN_BORDER);
    }

    if ( m_style & wxEL_ALLOW_DELETE )
    {
        m_bDel = new wxBitmapButton(subp, wxID_ELB_DELETE, wxBitmap(eldel_xpm));
        subsizer->Add(m_bDel, 0, wxALIGN_CENTRE_VERTICAL | wxTOP | wxBOTTOM, BTN_BORDER);
    }

    m_bUp = new wxBitmapButton(subp, wxID_ELB_UP, wxBitmap(elup_xpm));
    subsizer->Add(m_bUp, 0, wxALIGN_CENTRE_VERTICAL | wxTOP | wxBOTTOM, BTN_BORDER);

    m_bDown = new wxBitmapButton(subp, wxID_ELB_DOWN, wxBitmap(eldown_xpm));
    subsizer->Add(m_bDown, 0, wxALIGN_CENTRE_VERTICAL | wxTOP | wxBOTTOM, BTN_BORDER);

#if wxUSE_TOOLTIPS
    if ( m_bEdit ) m_bEdit->SetToolTip(wxT("Edit item"));
    if ( m_bNew ) m_bNew->SetToolTip(wxT("New item"));
    if ( m_bDel ) m_bDel->SetToolTip(wxT("Delete item"));
    m_bUp->SetToolTip(wxT("Move up"));
    m_bDown->SetToolTip(wxT("Move down"));
#endif

    subp->SetAutoLayout(TRUE);
    subp->SetSizer(subsizer);
    subsizer->Fit(subp);

    sizer->Add(subp, 0, wxEXPAND);

    long st = wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL | wxSUNKEN_BORDER;
    if ( style & wxEL_ALLOW_EDIT )
         st |= wxLC_EDIT_LABELS;
    m_listCtrl = new CleverListCtrl(this, wxID_ELD_LISTCTRL,
                                    wxDefaultPosition, wxDefaultSize, st);
    wxArrayString empty_ar;
    SetStrings(empty_ar);

    sizer->Add(m_listCtrl, 1, wxEXPAND);

    SetAutoLayout(TRUE);
    SetSizer(sizer);
    Layout();
}

void wxEditableListBox::SetStrings(const wxArrayString& strings)
{
    m_listCtrl->DeleteAllItems();
    size_t i;

    for (i = 0; i < strings.GetCount(); i++)
        m_listCtrl->InsertItem(i, strings[i]);

    m_listCtrl->InsertItem(strings.GetCount(), _T(""));
    m_listCtrl->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void wxEditableListBox::GetStrings(wxArrayString& strings)
{
    strings.Clear();

    for (int i = 0; i < m_listCtrl->GetItemCount()-1; i++)
        strings.Add(m_listCtrl->GetItemText(i));
}

void wxEditableListBox::OnItemSelected(wxListEvent& event)
{
    m_selection = event.GetIndex();
    m_bUp->Enable(m_selection != 0 && m_selection < m_listCtrl->GetItemCount()-1);
    m_bDown->Enable(m_selection < m_listCtrl->GetItemCount()-2);
    if (m_style & wxEL_ALLOW_EDIT)
        m_bEdit->Enable(m_selection < m_listCtrl->GetItemCount()-1);
    if (m_style & wxEL_ALLOW_DELETE)
        m_bDel->Enable(m_selection < m_listCtrl->GetItemCount()-1);
}

void wxEditableListBox::OnNewItem(wxCommandEvent& event)
{
    m_listCtrl->SetItemState(m_listCtrl->GetItemCount()-1,
                             wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    m_listCtrl->EditLabel(m_selection);
}

void wxEditableListBox::OnEndLabelEdit(wxListEvent& event)
{
    if ( event.GetIndex() == m_listCtrl->GetItemCount()-1 && 
         !event.GetText().IsEmpty() )
    {
        // The user edited last (empty) line, i.e. added new entry. We have to
        // add new empty line here so that adding one more line is still
        // possible:
        m_listCtrl->InsertItem(m_listCtrl->GetItemCount(), _T(""));
    }
}

void wxEditableListBox::OnDelItem(wxCommandEvent& event)
{
    m_listCtrl->DeleteItem(m_selection);
    m_listCtrl->SetItemState(m_selection,
                             wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void wxEditableListBox::OnEditItem(wxCommandEvent& event)
{
    m_listCtrl->EditLabel(m_selection);
}

void wxEditableListBox::OnUpItem(wxCommandEvent& event)
{
    wxString t1, t2;

    t1 = m_listCtrl->GetItemText(m_selection - 1);
    t2 = m_listCtrl->GetItemText(m_selection);
    m_listCtrl->SetItemText(m_selection - 1, t2);
    m_listCtrl->SetItemText(m_selection, t1);
    m_listCtrl->SetItemState(m_selection - 1,
                             wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void wxEditableListBox::OnDownItem(wxCommandEvent& event)
{
    wxString t1, t2;

    t1 = m_listCtrl->GetItemText(m_selection + 1);
    t2 = m_listCtrl->GetItemText(m_selection);
    m_listCtrl->SetItemText(m_selection + 1, t2);
    m_listCtrl->SetItemText(m_selection, t1);
    m_listCtrl->SetItemState(m_selection + 1,
                             wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}
