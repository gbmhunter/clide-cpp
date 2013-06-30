//!
//! @file 		Clide-Cmd.c
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @date 		2013/04/02
//! @brief 		Command-line style communications protocol
//! @details
//!		<b>Last Modified:			</b> 2013/05/14					\n
//!		<b>File Version:			</b> v1.0.0.0					\n
//!		<b>Company:					</b> CladLabs					\n
//!		<b>Project:					</b> Free Code Libraries		\n
//!		<b>Language:				</b> C++						\n
//!		<b>Compiler:				</b> GCC						\n
//! 	<b>uC Model:				</b> PSoC5						\n
//!		<b>Computer Architecture:	</b> ARM						\n
//! 	<b>Operating System:		</b> FreeRTOS v7.2.0			\n
//!		<b>Documentation Format:	</b> Doxygen					\n
//!		<b>License:					</b> GPLv3						\n
//!
//! See Clide-Cmd.h for more information.
//!

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//========================================= INCLUDES ============================================//
//===============================================================================================//

// System includes
#include <stdio.h>		// snprintf()
#include <stdlib.h>		// realloc(), malloc(), free()
#include <cctype>		// isalnum() 
#include <getopt.h>		// getopt()	

// User includes
#include "./Comms/include/UartDebug.h"
#include "./Comms/include/UartComms.h"
#include "MemMang.h"
#include "Clide-Config.h"
#include "Clide-Port.h"
#include "Clide-Option.h"
#include "Clide-Param.h"
#include "Clide-Cmd.h"

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace Clide
{


	//===============================================================================================//
	//==================================== PRIVATE DEFINES ==========================================//
	//===============================================================================================//

	// none

	//===============================================================================================//
	//=================================== PRIVATE TYPEDEF's =========================================//
	//===============================================================================================//

	// none

	//===============================================================================================//
	//============================= PRIVATE VARIABLES/STRUCTURES ====================================//
	//===============================================================================================//

	// none

	//===============================================================================================//
	//================================== PRIVATE FUNCTION PROTOTYPES ================================//
	//===============================================================================================//

	// none

	//===============================================================================================//
	//===================================== PUBLIC FUNCTIONS ========================================//
	//===============================================================================================//



	// Constructor
	Cmd::Cmd(const char *name, bool (*callBackFunc)(Cmd* foundCmd), const char *description)
	{
		#if(clideDEBUG_PRINT_VERBOSE == 1)	
			// Description too long, do not save it
			DebugPrint("CLIDE: Cmd constructor called.\r\n");
		#endif
		
		// INITIALISATION
		
		this->numParams = 0;
		this->numOptions = 0;
		
		// Set pointers to null. If not NULL, this causes realloc() is exhibit undefined behaviour,
		// since it expects a previously defined location or NULL
		this->paramA = NULL;
		this->optionA = NULL;

		// NAME
		
		uint32_t descLen = strlen(name);
		
		// Make sure the description isn't to long
		if(descLen <= clideMAX_NAME_LENGTH)
		{
			// Create memory for description and store
			this->name = (char*)MemMang::MallocString(name);
		}
		else
		{
			#if(clideDEBUG_PRINT_ERROR == 1)	
				// Description too long, do not save it
				DebugPrint("CLIDE: ERROR: Command name was too long.\r\n");
			#endif
		}
		
		// DECRIPTION
		
		descLen = strlen(description);
		
		// Make sure the description isn't to long
		if(descLen <= clideMAX_DESCRIPTION_LENGTH)
		{
			// Create memory for description and store
			this->description = MemMang::MallocString(description);
		}
		else
		{
			#if(clideDEBUG_PRINT_ERROR == 1)	
				// Description too long, do not save it
				DebugPrint("CLIDE: ERROR: Command description was too long.\r\n");
			#endif
		}
		
		// CALLBACK
		this->callBackFunc = callBackFunc;
		
		// HELP OPTION
		Option* help = new Option("h", NULL, "Prints help for the command.");
		this->RegisterOption(help);
		
	}

	void Cmd::RegisterParam(Param* param)
	{
		#if(clideDEBUG_PRINT_VERBOSE == 1)	
			// Description too long, do not save it
			DebugPrint("CLIDE: Registering parameter...\r\n");
		#endif

		//this->numParams = 0;
			
		// MALLOC
		this->paramA = (Param**)MemMang::AppendNewArrayElement(this->paramA, this->numParams, sizeof(Param*));
		
		if(this->paramA == NULL)
		{
			#if(clideDEBUG_PRINT_ERROR == 1)	
				// Description too long, do not save it
				DebugPrint("CLIDE: ERROR - Malloc failed while registering parameter.\r\n");
			#endif
			return;	
		}
		
		// Increase parameter count
		this->numParams++;
		
		// Remember the pointer
		paramA[numParams - 1] = param;
	}


	// Register option
	void Cmd::RegisterOption(Option* option)
	{
		#if(clideDEBUG_PRINT_VERBOSE == 1)	
			// Description too long, do not save it
			DebugPrint("CLIDE: Registering option...\r\n");
		#endif

		// Create option pointer at end of option pointer array.
		this->optionA = (Option**)MemMang::AppendNewArrayElement(this->optionA, this->numOptions, sizeof(Option*));
		
		// Increase option count
		this->numOptions++;
		
		// Remember option
		optionA[this->numOptions - 1] = option;
		
		#if(clideDEBUG_PRINT_VERBOSE == 1)	
			DebugPrint("CLIDE: Option name = ");
			DebugPrint(optionA[this->numOptions - 1]->name);
			DebugPrint("\r\n");
		#endif
	}


	//===============================================================================================//
	//==================================== PRIVATE FUNCTIONS ========================================//
	//===============================================================================================//

	// none

} // namespace Clide