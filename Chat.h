#pragma once
#include <vector>
#include <memory>
#include <exception>
#include "Message.h"
#include <iostream>

struct UserLoginExp : public std::exception
{
	const char* what() const noexcept override { return "Ошибка. Логин уже существует"; }
};
struct UserNameExp : public std::exception
{
	const char* what() const noexcept override { return "Ошибка: Имя пользователя уже существует"; }
};

class Chat
{
private:
	bool _isChatWork = false;
	std::vector<User> _users;
	std::vector<Message> _messages;
	std::shared_ptr<User> _currentUser = nullptr;

	void login();
	void signUp();
	void showChat() const;
	void showAllUsersName() const;
	void addMessage();
	std::vector<User>& getAllUsers() { return _users; }
	std::vector<Message>& getAllMessages() { return _messages; }
	std::shared_ptr<User> getUserByLogin(const std::string& login) const;
	std::shared_ptr<User> getUserByName(const std::string& name) const;


public:
	void start();
	bool isChatWork() const { return _isChatWork; }
	std::shared_ptr<User> getCurrentUser() const { return _currentUser; }
	void showLoginMenu();
	void showUserMenu();

};