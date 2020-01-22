/*  
 * pmacct (Promiscuous mode IP Accounting package)
 *
 * Copyright (c) 2003-2020 Paolo Lucente <paolo@pmacct.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* includes */
#include "pmacct.h"
#include "pmacct-data.h"
#include "redis_common.h"

/* Functions */
void p_redis_init(struct p_redis_host *redis_host, char *log_id, int nonblock)
{
  if (!redis_host || !log_id) return;

  memset(redis_host, 0, sizeof(struct p_redis_host));

  if (config.redis_host) {
    p_redis_set_log_id(redis_host, log_id);
 
    if (!config.cluster_name) {
      Log(LOG_ERR, "ERROR ( %s ): redis_host requires cluster_name to be specified. Exiting...\n\n", redis_host->log_id);
      exit_gracefully(1);
    }

    if (!config.cluster_id) {
      Log(LOG_ERR, "ERROR ( %s ): redis_host requires cluster_id to be specified. Exiting...\n\n", redis_host->log_id);
      exit_gracefully(1);
    }

    if (nonblock) {
      redis_host->ctx = redisConnectNonBlock(config.redis_host, PM_REDIS_DEFAULT_PORT);
    }
    else {
      redis_host->ctx = redisConnect(config.redis_host, PM_REDIS_DEFAULT_PORT); 
    }

    if (redis_host->ctx == NULL || redis_host->ctx->err) {
      if (redis_host->ctx) {
	Log(LOG_ERR, "ERROR ( %s ): Connection error to redist_host: %s\n", redis_host->log_id, redis_host->ctx->errstr);
        exit_gracefully(1);
      }
      else {
	Log(LOG_ERR, "ERROR ( %s ): Connection error to redis_host: can't allocate redis context\n", redis_host->log_id);
        exit_gracefully(1);
      }
    }
  }
}

void p_redis_set_log_id(struct p_redis_host *redis_host, char *log_id)
{
  if (redis_host) strlcpy(redis_host->log_id, log_id, sizeof(redis_host->log_id));
}
