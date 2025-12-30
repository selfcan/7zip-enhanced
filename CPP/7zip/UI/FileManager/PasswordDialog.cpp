// PasswordDialog.cpp

#include "StdAfx.h"

#include "PasswordDialog.h"

#ifdef Z7_LANG
#include "LangUtils.h"
#endif

#ifndef Z7_SFX
#include "../Common/ZipRegistry.h"
#endif

#ifdef Z7_LANG
static const UInt32 kLangIDs[] =
{
  IDT_PASSWORD_ENTER,
  IDX_PASSWORD_SHOW
};
#endif

void CPasswordDialog::ReadControls()
{
  _passwordCombo.GetText(Password);
  ShowPassword = IsButtonCheckedBool(IDX_PASSWORD_SHOW);
}

void CPasswordDialog::SetTextSpec()
{
  HWND editWindow = ::GetWindow(_passwordCombo, GW_CHILD);
  if (editWindow)
    ::SendMessage(editWindow, EM_SETPASSWORDCHAR, ShowPassword ? 0: TEXT('*'), 0);
  _passwordCombo.SetText(Password);
}

bool CPasswordDialog::OnInit()
{
  #ifdef Z7_LANG
  LangSetWindowText(*this, IDD_PASSWORD);
  LangSetDlgItems(*this, kLangIDs, Z7_ARRAY_SIZE(kLangIDs));
  #endif
  _passwordCombo.Attach(GetItem(IDC_PASSWORD_HISTORY));
  CheckButton(IDX_PASSWORD_SHOW, ShowPassword);
  SetTextSpec();

  #ifndef Z7_SFX
  UStringVector passwords;
  NExtract::Load_PasswordHistory(passwords);
  for (unsigned i = 0; i < passwords.Size(); i++)
  {
    _passwordCombo.AddString(passwords[i]);
  }
  #endif

  return CModalDialog::OnInit();
}

bool CPasswordDialog::OnButtonClicked(unsigned buttonID, HWND buttonHWND)
{
  if (buttonID == IDX_PASSWORD_SHOW)
  {
    ReadControls();
    SetTextSpec();
    return true;
  }
  return CDialog::OnButtonClicked(buttonID, buttonHWND);
}

void CPasswordDialog::OnOK()
{
  ReadControls();

  #ifndef Z7_SFX
  if (!Password.IsEmpty())
  {
    NExtract::Save_PasswordHistory(Password);
  }
  #endif

  CModalDialog::OnOK();
}
