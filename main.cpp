#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <functional>
using namespace std;

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
 // #define MIME_ADES
#include "mime2.h"
#pragma comment (lib,"crypt32.lib")

int main()
{
	// Single Message
	MIME2::CONTENT c;

	c["MIME-Version"] = "1.0";
	c["Content-Type"] = "text/plain";
	c.SetData("Hello");
	auto data = c.SerializeToVector();
	/*
MIME-Version: 1.0
Content-Type: text/plain

Hello
	*/

	// Binary
	c["MIME-Version"] = "1.0";
	c["Content-Type"] = "application/octet-stream";
	c["Content-Transfer-Encoding"] = "base64";
	c["Content-Disposition"] = "attachment; filename=\"hello.txt\"";
	string out = MIME2::Char2Base64("Hello", 5);
	c.SetData(out.c_str());
	data = c.SerializeToVector();

	/*
MIME-Version: 1.0
Content-Type: application/octet-stream
Content-Transfer-Encoding: base64
Content-Disposition: attachment; filename="hello.txt"

SGVsbG8=
	*/

	// Multipart
	MIME2::CONTENTBUILDER cb;
	MIME2::CONTENT e1;
	MIME2::CONTENT e2;
	e1["Content-Type"] = "text/plain";
	e1.SetData("Hello\r\n\r\n");
	e2["Content-Type"] = "text/html";
	e2.SetData("<b>Hello</b>");

	cb.Add(e1);
	cb.Add(e2);

	MIME2::CONTENT cc;
	cb.Build(cc, "multipart/alternative");

	data = cc.SerializeToVector();
	/*
MIME-Version: 1.0
Content-Type: multipart/alternative; boundary="{79EAC9E2-BAF9-11CE-8C82-00AA004BA90B}"

--{79EAC9E2-BAF9-11CE-8C82-00AA004BA90B}
Content-Type: text/plain

Hello

--{79EAC9E2-BAF9-11CE-8C82-00AA004BA90B}
Content-Type: text/html

<b>Hello</b>

--{79EAC9E2-BAF9-11CE-8C82-00AA004BA90B}--
	*/


}