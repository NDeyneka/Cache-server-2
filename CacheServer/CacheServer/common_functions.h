#pragma once
#include "headers.h"

/*!
* \file
* \author Sitdikov T. (str719@mail.ru)
* \date   April, 2019
* \brief  Common functions class
*
* \section DESCRIPTION_COMMON_FUNCTIONS
*
* common_functions - module containing some frequently used functions
* 1.	Copy string with memory allocation;
* 2.	Concat strings with memory allocation.
*/

//--------------------------------------------------
/// \brief  Frequently used functions class
///
/// This class contains some frequently used functions.
class common_functions
{
public:
	/// Allocate memory and copy string.
	/// \param[out]  dest  string-destination
	/// \param[in]  source  string-source
	static void alloc_and_copy(char **dest, const char *source);

	/// Allocate memory and concat two strings with delimeter and (possibly) endline.
	/// \param[out]  dest  string-destination
	/// \param[in]  source1  first string-source
	/// \param[in]  delimeter  string-delimeter
	/// \param[in]  source2  second string-source
	/// \param[in]  add_endline  flag for adding endline (0 - do not add endline, otherwise - add endline)
	static void alloc_and_concat(char **dest, const char * source1, const char * delimeter, const char *source2, int add_endline);
};

