#include <iostream>
#include "Chat.h"

void Chat::start()
{
	_isChatWork = true;
}

std::shared_ptr<User> Chat::getUserByLogin(const std::string& login) const
{
	for (auto& user : _users)
	{
		if (login == user.getUserLogin())
			return std::make_shared<User>(user);
	}

	return nullptr;
}

std::shared_ptr<User> Chat::getUserByName(const std::string& name) const
{
	for (auto& user : _users)
	{
		if (name == user.getUserName())
			return std::make_shared<User>(user);
	}

	return nullptr;
}

void Chat::login()
{
	std::string login, password;
	char operation;

	do
	{
		std::cout << "Пожалуйста, введите логин: " << std::endl;
		std::cin >> login;
		
		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "Пользователь не найден. " << std::endl;
		}
		else
		{
			std::cout << "Пожалуйста, введите пароль: " << std::endl;
			std::cin >> password;
		}

		if (_currentUser == nullptr || (password != _currentUser->getUserPassword()))
		{
			std::cout << "Не удалось авторизироваться. " << std::endl;
			std::cout << "Нажмите 0 (Выход) или любое слово для повтора: ";
			std::cin >> operation;

			if (operation == '0')
				break;
		}

	} while (!_currentUser);
	
}

void Chat::signUp()
{
	std::string  login, password, name;

	std::cout << "Введите логин: " << std::endl;
	std::cin >> login;
	std::cout << "Введите пароль: " << std::endl;
	std::cin >> password;
	std::cout << "Введите имя: " << std::endl;
	std::cin >> name;

	if (getUserByLogin(login) || login == "all")
	{
		throw UserLoginExp();
	}

	if (getUserByName(name) || name == "all")
	{
		throw UserNameExp();
	}

	User user = User(login, password, name);
	_users.push_back(user);
	_currentUser = std::make_shared<User>(user);
}

void Chat::showChat() const
{
	std::string from;
	std::string to;

	std::cout << " --- Чат --- " << std::endl;

	for (auto& mess : _messages)
	{
		// Показываем сообщения : от текущего пользователя, для него и для всех
		if (_currentUser->getUserLogin() == mess.getFrom() || _currentUser->getUserLogin() == mess.getTo() || mess.getTo() == "all")
		{
			//Подменяем для себя имя на я
			from = (_currentUser->getUserLogin() == mess.getFrom()) ? "меня" : getUserByLogin(mess.getFrom())->getUserName();

			if (mess.getTo() == "all")
			{
				to = "(all)";
			}
			else
			{
				to = (_currentUser->getUserLogin() == mess.getTo()) ? "меня" : getUserByLogin(mess.getTo())->getUserName();
			}


			std::cout << " Сообщение от " << from << " для " << to << std::endl;
			std::cout << " Сообщение: " << mess.getText()<< std::endl;
		}
	}

	std::cout << "-------------" << std::endl;
}

void Chat::showLoginMenu()
{
	_currentUser = nullptr;
	char operation;

	do
	{
		std::cout << "1. Авторизация" << std::endl;
		std::cout << "2. Регистрация" << std::endl;
		std::cout << "0. Завершение сеанса" << std::endl;
		std::cin >> operation;

		switch (operation)
		{
		case '1':
			login();
			break;
		case '2':
			try
			{
				signUp();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
			break;
		case '0':
			_isChatWork = false;
			break;
		default:
			std::cout << "1 или 2 ?" << std::endl;
			break;
		}
	} while (!_currentUser && _isChatWork);

}

void Chat::showUserMenu()
{
	char operation;
	std::cout << "Здравствуй, " << _currentUser->getUserName() << std::endl;

	while (_currentUser)
	{
		std::cout << "Меню: 1. Показать чат | 2. Отправить сообщение | 3. Пользователи | 0. Разлогиниться" <<std::endl;

		std::cin >> operation;

		switch (operation)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsersName();
			break;
		case '0':
			_currentUser = nullptr;
			break;
		default:
			std::cout << "Неизвестный выбор. Повторите попытку" << std::endl;
			break;
		}
	}
}

void Chat::showAllUsersName() const
{
	std::cout << "--- Пользователи ---" << std::endl;
	for (auto& user : _users)
	{
		std::cout << user.getUserName();

		if (_currentUser->getUserLogin() == user.getUserLogin())
			std::cout << "Я";

		std::cout << std::endl;
	}
	std::cout << "-------------" << std::endl;
}

void Chat::addMessage()
{
	std::string to, text;

	std::cout << "Кому (Имя или all(всем)): ";
	std::cin >> to;
	std::cout << "Сообщение: ";
	std::cin.ignore();
	getline(std::cin, text);

	if (!(to == "all" || getUserByName(to))) // Если не удалось найти получателя по имени
	{
		std::cout << "Невозможно отправить сообщение: пользователь не найден " << to << std::endl;
		return;
	}

	if (to == "all")
	{
		_messages.push_back(Message(_currentUser->getUserLogin(), "all", text));
	}
	else
		_messages.push_back(Message(_currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), text));
}