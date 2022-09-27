#include "Widgets/TextEditWidget.h"

FString UTextEditWidget::GetTextString() const
{
	return TextBox->GetText().ToString();
}
