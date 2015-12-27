#pragma once

enum class NetworkError
{
    Unknow = 0,

    CannotCreateSocket,
    ConnectTimeOut,
    ServerProcessNotExist,
    CannotFindRoute,
    LostConnection,
    RecvEmpty,
};