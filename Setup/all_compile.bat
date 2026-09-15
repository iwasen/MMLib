pause
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMLIBJ32.iss
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMLIBJ64.iss
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMLIBE32.iss
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMLIBE64.iss

"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMViewJ32.iss
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMViewJ64.iss
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMViewE32.iss
"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc MMViewE64.iss

copy /y Installer\* W:\ymec\ystore\admin\download\
scp Installer\* aizawa@ymec.com:/htdocs/ikou_www/ystore/admin/download/

pause