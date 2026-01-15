#pragma once

struct NetworkMessage
{
	enum class Type
	{
		invalidType,
		clientRegistration,
		clientAdded,
		replicateExistingClients,
		clientDisconnected,
		clientChangedName,
		clientSentMessage
	};
};
