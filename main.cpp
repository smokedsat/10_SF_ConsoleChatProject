#include "Chat.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	Chat chat;

	chat.start();

	while (chat.isChatWork())
	{
		chat.showLoginMenu();

		while (chat.getCurrentUser())
		{
			chat.showUserMenu();
		}
	}

	return 0;
}