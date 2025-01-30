#ifndef UI_CLIENTDC_H
#define UI_CLIENTDC_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_DC.h"

// Classes /////////////////////////////////////////////////////////////////

namespace UI
{
	class Window;
	class Control;

	class ClientDC : public DC
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		ClientDC(const Window &window);
		ClientDC(const Control &control);
		ClientDC(const ClientDC &other);
		ClientDC &operator=(ClientDC other);
		~ClientDC();

		////////////////////////////////////////////////////////////////////

	private:

		////////////////////////////////////////////////////////////////////
		// Private /////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Init Functions //////////////////////////////////////////////////

		HDC InitHandle(HWND window) const;

		// Variables ///////////////////////////////////////////////////////

		HWND window;
		int *references;

		////////////////////////////////////////////////////////////////////
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
