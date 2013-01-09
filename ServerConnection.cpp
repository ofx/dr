#include "StdAfx.h"

#include "ServerConnection.h"

#include <tchar.h>
#include <strsafe.h>

int Packet::m_MessageId = 0;

Packet::Packet(void)
{
    this->m_Data = cJSON_CreateObject();

    // Add the message id
    this->AddNumberItem("mid", this->m_MessageId++);

    // Add the protocol version
    this->AddNumberItem("protocol", PROTOCOL_VERSION);
}

Packet::~Packet(void)
{
    cJSON_Delete(this->m_Data); this->m_Data = 0;
}

void Packet::FromString(const char *data)
{
    cJSON_Delete(this->m_Data);
    this->m_Data = cJSON_Parse(data);

    // Set protocol version
    int protocol = -1;
    if(!this->GetIntValue("protocol", protocol))
    {
        // TODO: Handle me properly
        __asm
        {
            int 13
        }
    }
    else
    {
        this->m_Protocol = protocol;
    }

    // Set packet type
    std::string type;
    if(this->GetStringValue("type", type))
    {
        if(type == "error")
        {
            this->SetPacketType(Error);
        }
        else if(type == "request")
        {
            this->SetPacketType(Request);
        }
        else if(type == "reply")
        {
            this->SetPacketType(Response);
        }
    }
}

void Packet::DeleteItem(std::string item)
{
    cJSON_DeleteItemFromObject(this->m_Data, item.c_str());
}

void Packet::AddNullItem(std::string item)
{
    cJSON_AddNullToObject(this->m_Data, item.c_str());
}

void Packet::AddTrueItem(std::string item)
{
    cJSON_AddTrueToObject(this->m_Data, item.c_str());
}

void Packet::AddFalseItem(std::string item)
{
    cJSON_AddFalseToObject(this->m_Data, item.c_str());
}

void Packet::AddNumberItem(std::string item, int value)
{
    cJSON_AddNumberToObject(this->m_Data, item.c_str(), value);
}
void Packet::AddStringItem(std::string item, std::string value)
{
    cJSON_AddStringToObject(this->m_Data, item.c_str(), value.c_str());
}

bool Packet::IsItemSet(std::string item)
{
    cJSON *i = cJSON_GetObjectItem(this->m_Data, item.c_str());

    return i == 0;
}

bool Packet::GetBoolValue(std::string item, bool &value)
{
    cJSON *i = cJSON_GetObjectItem(this->m_Data, item.c_str());
    
    __asm
    {
        int 13
    };
}

bool Packet::GetIntValue(std::string item, int &value)
{
    cJSON *i = cJSON_GetObjectItem(this->m_Data, item.c_str());
    
    if(i != 0)
    {
        value = i->valueint;

        return true;
    }
    else
    {
        return false;
    }
}

bool Packet::GetStringValue(std::string item, std::string &value)
{
    cJSON *i = cJSON_GetObjectItem(this->m_Data, item.c_str());
    
    if(i != 0)
    {
        value = i->valuestring;

        return true;
    }
    else
    {
        return false;
    }
}

bool Packet::GetFloatValue(std::string item, float &value)
{
    cJSON *i = cJSON_GetObjectItem(this->m_Data, item.c_str());

    if(i != 0)
    {
        value = i->valuedouble;

        return true;
    }
    else
    {
        return false;
    }
}

void Packet::SetPacketType(PacketType type)
{
    switch(type)
    {
    case Request:
        this->AddStringItem("type", "request");
        this->m_Type = Request;
        break;
    case Response:
        this->AddStringItem("type", "reply");
        this->m_Type = Response;
        break;
    case Error:
        this->AddStringItem("type", "error");
        this->m_Type = Error;
        break;
    }
}

PacketType Packet::GetPacketType(void)
{
    return this->m_Type;
}

void Packet::ConstructErrorPacket(std::string errorMessage)
{
    this->SetPacketType(Error);
    this->AddStringItem("error", errorMessage);
}

void Packet::ConstructPingPacket(std::string cid, std::string gid, PacketType type)
{
	this->SetPacketType(type);
	this->AddStringItem("header", "ping");
	this->AddStringItem("cid", cid);
	this->AddStringItem("gid", gid);
}

char *Packet::GetString(void)
{
    return cJSON_Print(this->m_Data);
}

ServerConnection::ServerConnection(Engine *engine)
{
    // Store a reference to the parent engine
    this->m_Engine = engine;

    // Load the required connection information
    this->m_ServerAddress = this->m_Engine->GetHge()->Ini_GetString("ServerConnection", "Address", "127.0.0.1");
    this->m_ServerPort    = this->m_Engine->GetHge()->Ini_GetString("ServerConnection", "Port",    "7223");

    this->m_ServerPort    = "7223";
    this->m_ServerAddress = "127.0.0.1";
    
    // Define address components
    unsigned int a[4];
    unsigned int i = 0;

    a[0] = 127;
    a[1] = 0;
    a[2] = 0;
    a[3] = 1;

    // Define address and port
    unsigned int   address = (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
    unsigned short port    = atoi(this->m_ServerPort);

    // Set the server address struct
    this->m_ServerSockAddress = new sockaddr_in();
    this->m_ServerSockAddress->sin_family      = AF_INET;
    this->m_ServerSockAddress->sin_addr.s_addr = htonl(address);
    this->m_ServerSockAddress->sin_port        = htons(port);

    // Initialize the SocketHandle
    this->Initialize();
}

ServerConnection::~ServerConnection(void)
{
    // Delete stuff
    delete this->m_ServerSockAddress;

    // Delete the texture and sprite
    this->m_Engine->GetHge()->Texture_Free(this->m_QrCode);
    delete this->m_QrCodeSprite;
    
    // Close and cleanup
    this->CloseSockets();
}

bool ServerConnection::InitializeSockets(void)
{
    // Startup WSA
    WSADATA WsaData;
    return WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
}

void ServerConnection::CloseSockets(void)
{
    // Close the socket
    closesocket(this->m_SocketHandle);

    // WSA cleanup
    WSACleanup();
}

bool ServerConnection::CreateSockets(void)
{
    // Create the socket handles
    this->m_SocketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Check if we've received a valid handle
    if (this->m_SocketHandle <= 0)
    {
        this->m_Engine->GetHge()->System_Log("Could not get socket handle, cannot continue");
        this->m_Engine->Shutdown();
    }

    // Bind to some port on any address to receive response packets
    sockaddr_in address;
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = 0;

    // Try binding socket
    if (bind(this->m_SocketHandle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
    {
        this->m_Engine->GetHge()->System_Log("Could not bind socket, cannot continue");
        this->m_Engine->Shutdown();
    }

    // Indicate non-blocking
    DWORD nonBlocking = 1;
    if (ioctlsocket(this->m_SocketHandle, FIONBIO, &nonBlocking) != 0)
    {
        this->m_Engine->GetHge()->System_Log("Could not set non-blocking, cannot continue");
        this->m_Engine->Shutdown();
    }

    return true;
}

bool ServerConnection::SendData(const char *data, unsigned int size, sockaddr *address)
{
    // Send a packet with data of size to address
    int sent = sendto(this->m_SocketHandle, data, size, 0, address, sizeof(sockaddr_in));

    return sent == size;
}

void ServerConnection::NewGame( Packet &packet )
{
    std::string gid; packet.GetStringValue("gid", gid);
    int mid;         packet.GetIntValue("mid", mid);

    // Set the game id
    this->m_GameUuid = gid;

    QRcode *qrcode;
    qrcode = QRcode_encodeString(gid.c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 0);

    // Create a texture
    this->m_QrCode = this->m_Engine->GetHge()->Texture_Create(qrcode->width, qrcode->width);

    // Lock the texture
    DWORD *pix = this->m_Engine->GetHge()->Texture_Lock(this->m_QrCode, false);

    for( int y = 0 ; y < qrcode->width ; ++y )
    {
        for( int x = 0 ; x < qrcode->width ; ++x )
        {
            pix[y * qrcode->width + x] = qrcode->data[y * qrcode->width + x] & 1 ? 0xFF000000 : 0xFFFFFFFF;
        }
    }

    // Unlock the texture
    this->m_Engine->GetHge()->Texture_Unlock(this->m_QrCode);

    // Create a sprite for the QR code (I acknowledge the fact that this isn't quite the location to do this
    // sort of stuff)
    this->m_QrCodeSprite = new hgeSprite(this->m_QrCode, 0, 0, qrcode->width, qrcode->width);

    QRcode_free(qrcode);
}

void ServerConnection::Dispatch(char *data, unsigned int size)
{
    // Create a packet from the data
    Packet in;
    in.FromString(data);

    // Retrieve the client id
    std::string cid; bool hasCid = in.GetStringValue( "cid", cid );

    // In case we've received an error packet, log directly to our log
    if (in.GetPacketType() == Error || !hasCid)
    {
        std::string errorMessage; in.GetStringValue("error", errorMessage);
        this->m_Engine->GetHge()->System_Log(errorMessage.c_str());
    }
    // Else check what we should do
    else
    {
        // Retrieve the header
        std::string header;
        if( !in.GetStringValue( "header", header ) )
        {
            // Not a valid header, ignore
#ifdef _DEBUG
            __asm
            {
                int 13
            }
#endif
        }

        // Retrieve the type
        std::string type; in.GetStringValue("type", type);

        // Check packet
		if( type == "inform" )
		{
			//this->DispatchClientUpdate(packet);
		}
        else if( type == "request" )
        {
            if(header == "throttle" || header == "-attack1" || header == "+attack1")
            {
                //this->DispatchClientRequest(packet);
            }
            else if(header == "joinGame")
		    {
			    //this->JoinGame(packet);
		    }
        }
        else if(header == "newGame")
        {
            this->NewGame(in);
        }
		else if(header == "ping")
		{
			if(type == "request")
			{
                Packet ping;
			    ping.ConstructPingPacket(cid, this->m_GameUuid, Response);

                char *data  = ping.GetString();
                DWORD datal = strlen(data);
                this->SendData(data, datal, (sockaddr*) this->m_ServerSockAddress);
			}
		}
    }
}

bool ServerConnection::Initialize(void)
{
    // Initialize the WinSock layer
    if (!this->InitializeSockets())
    {
        // Ouch...
        this->m_Engine->GetHge()->System_Log("Could not initialize socket, cannot continue");
        this->m_Engine->Shutdown();
    }

    // Create sockets
    this->CreateSockets();

    // Try performing the handshake and await result, since we should not yet have
    // started the game at this point
    Packet handshake;
    handshake.SetPacketType(Request);
    handshake.AddStringItem("type", "request");
    handshake.AddStringItem("header", "newGame");
    handshake.AddStringItem("location", "-unimplemented-");

    // Write
    char *data  = handshake.GetString();
    DWORD datal = strlen(handshake.GetString());
    this->SendData(data, datal, (sockaddr*) this->m_ServerSockAddress);

    return true;
}

void ServerConnection::ReadAndDispatchData(void)
{
    unsigned char data[256];
    unsigned int size = sizeof(data);

    typedef int socklen_t;
        
    sockaddr_in from;
    socklen_t fromLength = sizeof( from );

    // Try reading
    int recv = recvfrom(this->m_SocketHandle, (char*)data, size, 0, (sockaddr*)&from, &fromLength);

    // In case we've received data, dispatch the data
    if (recv > 0)
    {
        this->Dispatch((char *)data, size);
    }
}

void ServerConnection::RenderQrCode(void)
{
    // Render the QR code
    this->m_QrCodeSprite->RenderStretch(this->m_Engine->GetWidth() - 220, this->m_Engine->GetHeight() - 220, this->m_Engine->GetWidth() - 20, this->m_Engine->GetHeight() - 20);
}