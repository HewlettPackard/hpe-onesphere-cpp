*Note: This library is not currently being maintained. We will still maintain reviewing pull requests but active development for this library has paused.* 

# C++ Binding for HPE OneSphere APIs

The C++ binding package for HPE OneSphere APIs.

## Prerequisites

Install the nlohmann JSON library:

```
https://github.com/nlohmann/json
```

And install libcurl:

```
https://curl.haxx.se/libcurl/
```

## Usage

Copy all the .cpp and .h files into your project folder and use #include "osbinding.h".

Example:

```
#include "osbinding.h"
#include <iostream>

using namespace std;

int main() {
    OSClient client("https://onesphere-host-url", "username", "password");

    cout << "GetStatus: " + (client.GetStatus()).dump() << endl;
    cout << "GetConnectApp: " + (client.GetConnectApp()).dump() << endl;
    cout << "GetSession: " + (client.GetSession()).dump() << endl;
    cout << "GetAccount: " + (client.GetAccount()).dump() << endl;
    cout << "GetProviderTypes: " + (client.GetProviderTypes()).dump() << endl;
    cout << "GetZoneTypes: " + (client.GetZoneTypes()).dump() << endl;
    cout << "GetServiceTypes: " + (client.GetServiceTypes()).dump() << endl;
    cout << "GetRoles: " + (client.GetRoles()).dump() << endl;
    cout << "GetUsers: " + (client.GetUsers()).dump() << endl;
    cout << "GetTagKeys: " + (client.GetTagKeys()).dump() << endl;
    cout << "GetTags: " + (client.GetTags()).dump() << endl;

    return 0;
}
```
