@ stdcall CredPackAuthenticationBufferW(long wstr ptr ptr ptr) credui.CredPackAuthenticationBufferW
@ stdcall CredUICmdLinePromptForCredentialsW(wstr ptr long wstr long wstr long ptr long) credui.CredUICmdLinePromptForCredentialsW
@ stdcall CredUIConfirmCredentialsA(wstr long) credui.CredUIConfirmCredentialsA
@ stdcall CredUIConfirmCredentialsW(wstr long) credui.CredUIConfirmCredentialsW
@ stdcall CredUIPromptForCredentialsW(ptr wstr ptr long ptr long ptr long ptr long) credui.CredUIPromptForCredentialsW
@ stdcall CredUIPromptForWindowsCredentialsW(ptr long ptr ptr long ptr ptr ptr long) credui.CredUIPromptForWindowsCredentialsW
@ stdcall CredUnPackAuthenticationBufferW(long ptr long ptr ptr ptr ptr ptr ptr) credui.CredUnPackAuthenticationBufferW
@ stub SspiIsPromptingNeeded
@ stub SspiPromptForCredentialsW ;(wstr ptr long wstr ptr ptr ptr long) credui.SspiPromptForCredentialsW
