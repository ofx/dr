#pragma once

#define PROTOCOL_VERSION          0

#define RESPONSE_TIMEOUT          10000
#define RESPONSE_BUFFER_SIZE      1024

#define SERVER_CONNECTION_TIMEOUT 20

#include <string>

enum PacketType
{
    Request,
    Response,
    Error
};

class Packet
{
private:
    cJSON *m_Data;

    static int m_MessageId;

    int m_Protocol;

    PacketType m_Type;
public:
    Packet( void );
    ~Packet( void );

    void FromString( const char * );
    void DeleteItem( std::string );

    void AddNullItem( std::string );
    void AddTrueItem( std::string );
    void AddFalseItem( std::string );
    void AddNumberItem( std::string, int );
    void AddStringItem( std::string, std::string );

    bool IsItemSet( std::string );

    bool GetBoolValue( std::string, bool & );
    bool GetIntValue( std::string, int & );
    bool GetFloatValue( std::string, float & );
    bool GetStringValue( std::string, std::string & );
    
    PacketType GetPacketType( void );
    char *GetString( void );

    void SetPacketType( PacketType );

    void ConstructErrorPacket( std::string );
    void ConstructPingPacket( std::string, std::string, PacketType );
};

class ServerConnection
{
private:
    sockaddr_in *m_ServerSockAddress;
    int          m_SocketHandle;

    Engine      *m_Engine;

    HTEXTURE     m_QrCode;
    hgeSprite   *m_QrCodeSprite;

    std::string  m_GameUuid;

    char        *m_ServerAddress;
    char        *m_ServerPort;

    bool         m_IsConnected;

    time_t       m_LastTransmission;

    void EncodeQrcodeToPng(QRcode *qrcode);

    bool InitializeSockets(void);
    bool CreateSockets(void);
    void CloseSockets(void);

    void ReadAndDispatchData(void);
    void Dispatch(char *data, unsigned int size);
    bool SendData(const char *data, unsigned int size, sockaddr *address);

    bool Initialize(void);

    void NewGame(Packet &packet);
public:
    ServerConnection(Engine *engine);
    ~ServerConnection(void);

    bool IsConnected(void) {return this->m_IsConnected;}

    void Update(float dt);
    void Render(hgeFont *font, float dt);
    void RenderQrCode(void);
};

