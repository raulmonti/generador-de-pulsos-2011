//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Project1.res");
USEFORM("interfaz.cpp", formPrincipal);
USEUNIT("instruction.cpp");
USEUNIT("instruction_list.cpp");
USEUNIT("phase.cpp");
USEUNIT("..\DLPortIO\dlportio\cbuilder\cbuilder.4\DLPortIO.cpp");
USEUNIT("..\DLPortIO\dlportio\cbuilder\cbuilder.4\TDLPortIO.cpp");
USEUNIT("phase_list.cpp");
USEFORM("formGrafico.cpp", formGraficos);
USE("error.h", File);
USEUNIT("error.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TformPrincipal), &formPrincipal);
                 Application->CreateForm(__classid(TformGraficos), &formGraficos);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
