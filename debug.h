/*! \file    debug.h
 * \author   Lorenzo Miniero <lorenzo@meetecho.com>
 * \copyright GNU General Public License v3
 * \brief    Logging and Debugging
 * \details  Implementation of a wrapper on printf (or g_print) to either log or debug.
 *
 * \ingroup core
 * \ref core
 */

#ifndef JANUS_DEBUG_H
#define JANUS_DEBUG_H

#include <glib.h>
#include <glib/gprintf.h>
#include "log.h"

extern int janus_log_level;
extern gboolean janus_log_timestamps;
extern gboolean janus_log_colors;
extern char *janus_log_global_prefix;

/** @name Janus log colors
 */
///@{
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
///@}

/** @name Janus log levels
 */
///@{
/*! \brief No debugging */
#define LOG_NONE     (0)
/*! \brief Fatal error */
#define LOG_FATAL    (1)
/*! \brief Non-fatal error */
#define LOG_ERR      (2)
/*! \brief Warning */
#define LOG_WARN     (3)
/*! \brief Informational message */
#define LOG_INFO     (4)
/*! \brief Verbose message */
#define LOG_VERB     (4)
/*! \brief Overly verbose message */
#define LOG_HUGE     (4)
/*! \brief Debug message (includes .c filename, function and line number) */
#define LOG_DBG      (7)
/*! \brief Maximum level of debugging */
#define LOG_MAX LOG_DBG

/*! \brief Coloured prefixes for errors and warnings logging. */
static const char *janus_log_prefix[] = {
/* no colors */
	"",
	"[FATAL] ",
	"[ERR] ",
	"[WARN] ",
	"",
	"",
	"",
	"",
/* with colors */
	"",
	ANSI_COLOR_MAGENTA "[FATAL]" ANSI_COLOR_RESET " ",
	ANSI_COLOR_RED "[ERR]" ANSI_COLOR_RESET " ",
	ANSI_COLOR_YELLOW "[WARN]" ANSI_COLOR_RESET " ",
	"",
	"",
	"",
	""
};
///@}

/** @name Janus log wrappers
 */
///@{
/*! \brief Simple wrapper to g_print/printf */
#define JANUS_PRINT janus_vprintf
/*! \brief Logger based on different levels, which can either be displayed
 * or not according to the configuration of the server.
 * The format must be a string literal. */
#define JANUS_LOG(level, format, ...) \
do { \
	if (level > LOG_NONE && level <= LOG_MAX && level <= janus_log_level) { \
		char janus_log_ts[64] = ""; \
		char janus_log_src[128] = ""; \
		if (janus_log_timestamps) { \
			struct tm janustmresult; \
			time_t janusltime = time(NULL); \
			localtime_r(&janusltime, &janustmresult); \
			strftime(janus_log_ts, sizeof(janus_log_ts), \
			         "[%a %b %e %T %Y] ", &janustmresult); \
		} \
		if (level == LOG_FATAL || level == LOG_ERR || level == LOG_DBG) { \
			snprintf(janus_log_src, sizeof(janus_log_src), \
			         "[%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__); \
		} \
		JANUS_PRINT("%s%s%s%s" format, \
			janus_log_global_prefix ? janus_log_global_prefix : "", \
			janus_log_ts, \
			janus_log_prefix[level | ((int)janus_log_colors << 3)], \
			janus_log_src, \
			##__VA_ARGS__); \
	} \
} while (0)
///@}

typedef struct room_info {
	guint64 room_id;			/* Unique room ID (when using integers) */
	gchar *room_id_str;			/* Unique room ID (when using strings) */
	gchar *room_name_of_brower1;			/* Room description */
	gchar *room_name_of_brower2;			/* Room description */
	gchar *transaction_text_of_brower1;
	gchar *transaction_text_of_brower2;
	guint64 *session_id;
	gchar *user_id_str_of_brower1;	/* feed_id, Unique ID in the room (when using strings) */
	gchar *user_id_str_of_brower2;	/* feed_id, Unique ID in the room (when using strings) */
	char *sdp_offer_request_configure; /* Sdp offer with request "configure" from brower, sdp_offer[0] for brower1, sdp_offer[1] for brower 2*/
	char *sdp_answer_request_start; /* Sdp answer with request "start" from brower, sdp_answer[0] for brower1, sdp_answer[1] for brower 2*/
	gboolean isCompletedCandidateOfBrower1; /*Brower1 sent "Completed Candidate"*/
	gboolean isCompletedCandidateOfBrower2; /*Brower1 sent "Completed Candidate"*/
	gboolean isSubscriberOfBrower2;
	gboolean isSubscriberOfBrower1;

	struct room_info *next; /*pointer to next element*/
} room_info;

extern room_info *roomInfo;
extern int typeOfMessage;
extern gboolean enableSdpStart;

#endif

