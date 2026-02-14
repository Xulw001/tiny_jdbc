/**
 * @file platform.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the platform-specific definitions for SQL operations
 * @version 0.1
 * @date 2026-02-10
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_PLATFORM_H
#define SQL_PLATFORM_H

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)

#ifdef __cplusplus
#define EXPORT_C extern "C" EXPORT_API
#else
#define EXPORT_C EXPORT_API
#endif

#else
#if __GNUC__ >= 4
#define EXPORT_API __attribute__((visibility("default")))
#else
#define EXPORT_API
#endif

#ifdef __cplusplus
#define EXPORT_C extern "C" EXPORT_API
#else
#define EXPORT_C EXPORT_API
#endif
#endif

#ifdef _WIN32
#include <windows.h>
#define DL_HANDLE HMODULE
#define DL_OPEN(path) LoadLibraryA(path)
#define DL_SYM(handle, name) GetProcAddress(handle, name)
#define DL_CLOSE(handle) FreeLibrary(handle)
#define SUFFIX ".dll"
#else
#include <dlfcn.h>
#define DL_HANDLE void*
#define DL_OPEN(path) dlopen(path, RTLD_LAZY)
#define DL_SYM(handle, name) dlsym(handle, name)
#define DL_CLOSE(handle) dlclose(handle)
#define SUFFIX ".so"
#endif

#endif