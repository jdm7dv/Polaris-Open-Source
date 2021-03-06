// Copyright 2010 Hewlett-Packard under the terms of the MIT X license
// found at http://www.opensource.org/licenses/mit-license.html

#include "stdafx.h"
#include "OpenPetURLHandler.h"

#include "MessageHandler.h"
#include "MessageResponder.h"
#include "../polaris/Message.h"
#include "../polaris/Request.h"
#include "../polaris/RPCClient.h"

class OpenPetURLHandler : public MessageHandler
{
public:
    OpenPetURLHandler() {}
    virtual ~OpenPetURLHandler() {}

    virtual void run(Message& request, MessageResponder* out) {

        // Deserialize the command line arguments.
        wchar_t* cmd_line;
        request.cast(&cmd_line, 0);
        int argc = 0;
        wchar_t** argv = CommandLineToArgvW(cmd_line, &argc);

        // The only argument is the URL to open.
        if (argc == 1) {
            // TODO: Really important to check that the argument is actually a URL!!!
	        RPCClient* client = makeClient();
            Request* request = client->initiate("openurl", (wcslen(cmd_line) + 1) * sizeof(wchar_t));
            request->send(cmd_line);
            request->finish(false, 60);
            DWORD r = 0;
            out->run(sizeof r, &r);
        } else {
            out->run(0, NULL);
        }
    }
};

MessageHandler* makeOpenPetURLHandler() {
    return new OpenPetURLHandler();
}