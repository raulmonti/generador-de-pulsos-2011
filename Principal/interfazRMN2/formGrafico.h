//---------------------------------------------------------------------------

#ifndef formGraficoH
#define formGraficoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TformGraficos : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TEdit *TxtMin;
        TEdit *TxtMax;
        TLabel *Label1;
        TLabel *Label2;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TformGraficos(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformGraficos *formGraficos;
//---------------------------------------------------------------------------
#endif
