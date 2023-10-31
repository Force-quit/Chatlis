#pragma once

enum class NetworkMessageType
{
	invalidType,
	clientRegistration,
	clientAdded,
	replicateExistingClients,
	clientDisconnected,
	clientChangeUsername,
	clientChangeComputerName,
	clientSentMessage
};