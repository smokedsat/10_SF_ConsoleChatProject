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
		std::cout << "����������, ������� �����: " << std::endl;
		std::cin >> login;
		
		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "������������ �� ������. " << std::endl;
		}
		else
		{
			std::cout << "����������, ������� ������: " << std::endl;
			std::cin >> password;
		}

		if (_currentUser == nullptr || (password != _currentUser->getUserPassword()))
		{
			std::cout << "�� ������� ����������������. " << std::endl;
			std::cout << "������� 0 (�����) ��� ����� ����� ��� �������: ";
			std::cin >> operation;

			if (operation == '0')
				break;
		}

	} while (!_currentUser);
	
}

void Chat::signUp()
{
	std::string  login, password, name;

	std::cout << "������� �����: " << std::endl;
	std::cin >> login;
	std::cout << "������� ������: " << std::endl;
	std::cin >> password;
	std::cout << "������� ���: " << std::endl;
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

	std::cout << " --- ��� --- " << std::endl;

	for (auto& mess : _messages)
	{
		// ���������� ��������� : �� �������� ������������, ��� ���� � ��� ����
		if (_currentUser->getUserLogin() == mess.getFrom() || _currentUser->getUserLogin() == mess.getTo() || mess.getTo() == "all")
		{
			//��������� ��� ���� ��� �� �
			from = (_currentUser->getUserLogin() == mess.getFrom()) ? "����" : getUserByLogin(mess.getFrom())->getUserName();

			if (mess.getTo() == "all")
			{
				to = "(all)";
			}
			else
			{
				to = (_currentUser->getUserLogin() == mess.getTo()) ? "����" : getUserByLogin(mess.getTo())->getUserName();
			}


			std::cout << " ��������� �� " << from << " ��� " << to << std::endl;
			std::cout << " ���������: " << mess.getText()<< std::endl;
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
		std::cout << "1. �����������" << std::endl;
		std::cout << "2. �����������" << std::endl;
		std::cout << "0. ���������� ������" << std::endl;
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
			std::cout << "1 ��� 2 ?" << std::endl;
			break;
		}
	} while (!_currentUser && _isChatWork);

}

void Chat::showUserMenu()
{
	char operation;
	std::cout << "����������, " << _currentUser->getUserName() << std::endl;

	while (_currentUser)
	{
		std::cout << "����: 1. �������� ��� | 2. ��������� ��������� | 3. ������������ | 0. �������������" <<std::endl;

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
			std::cout << "����������� �����. ��������� �������" << std::endl;
			break;
		}
	}
}

void Chat::showAllUsersName() const
{
	std::cout << "--- ������������ ---" << std::endl;
	for (auto& user : _users)
	{
		std::cout << user.getUserName();

		if (_currentUser->getUserLogin() == user.getUserLogin())
			std::cout << "�";

		std::cout << std::endl;
	}
	std::cout << "-------------" << std::endl;
}

void Chat::addMessage()
{
	std::string to, text;

	std::cout << "���� (��� ��� all(����)): ";
	std::cin >> to;
	std::cout << "���������: ";
	std::cin.ignore();
	getline(std::cin, text);

	if (!(to == "all" || getUserByName(to))) // ���� �� ������� ����� ���������� �� �����
	{
		std::cout << "���������� ��������� ���������: ������������ �� ������ " << to << std::endl;
		return;
	}

	if (to == "all")
	{
		_messages.push_back(Message(_currentUser->getUserLogin(), "all", text));
	}
	else
		_messages.push_back(Message(_currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), text));
}