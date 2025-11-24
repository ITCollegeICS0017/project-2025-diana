
#pragma once
#include <stdexcept>

class AppException : public std::runtime_error { using std::runtime_error::runtime_error; };
class ValidationException : public AppException { using AppException::AppException; };
class RepositoryException : public AppException { using AppException::AppException; };
class ServiceException : public AppException { using AppException::AppException; };
