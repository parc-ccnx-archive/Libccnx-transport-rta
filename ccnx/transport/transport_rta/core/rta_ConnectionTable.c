/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
/*
 * Uses a linked list right now, but should be hash tables on the keys we use.
 */
#include <config.h>
#include <stdio.h>
#include <sys/queue.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <LongBow/runtime.h>
#include <parc/algol/parc_Memory.h>

#include <ccnx/transport/transport_rta/core/rta_Framework_Services.h>
#include <ccnx/transport/transport_rta/core/rta_ConnectionTable.h>

#define DEBUG_OUTPUT 0

typedef struct rta_connection_entry {
    RtaConnection *connection;

    TAILQ_ENTRY(rta_connection_entry) list;
} RtaConnectionEntry;

struct rta_connection_table {
    size_t max_elements;
    size_t count_elements;
    TableFreeFunc *freefunc;
    TAILQ_HEAD(, rta_connection_entry) head;
};


/**
 * Create a connection table of the given size
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnectionTable *
rtaConnectionTable_Create(size_t elements, TableFreeFunc *freefunc)
{
    RtaConnectionTable *table = parcMemory_AllocateAndClear(sizeof(RtaConnectionTable));
    assertNotNull(table, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(RtaConnectionTable));
    TAILQ_INIT(&table->head);
    table->max_elements = elements;
    table->count_elements = 0;
    table->freefunc = freefunc;
    return table;
}

/**
 * Destroy the connection table, and it will call rtaConnection_Destroy()
 * on each connection in the table.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void
rtaConnectionTable_Destroy(RtaConnectionTable **tablePtr)
{
    RtaConnectionTable *table;

    assertNotNull(tablePtr, "Called with null parameter");
    table = *tablePtr;
    assertNotNull(table, "Called with parameter that dereferences to null");

    while (!TAILQ_EMPTY(&table->head)) {
        RtaConnectionEntry *entry = TAILQ_FIRST(&table->head);
        TAILQ_REMOVE(&table->head, entry, list);
        if (table->freefunc) {
            table->freefunc(&entry->connection);
        }
        parcMemory_Deallocate((void **) &entry);
    }

    parcMemory_Deallocate((void **) &table);
    *tablePtr = NULL;
}

/**
 * Add a connetion to the table.  Stores the reference provided (does not copy).
 * Returns 0 on success, -1 on error
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
rtaConnectionTable_AddConnection(RtaConnectionTable *table, RtaConnection *connection)
{
    assertNotNull(table, "Called with null parameter RtaConnectionTable");
    assertNotNull(connection, "Called with null parameter RtaConnection");

    if (table->count_elements < table->max_elements) {
        table->count_elements++;
        RtaConnectionEntry *entry = parcMemory_AllocateAndClear(sizeof(RtaConnectionEntry));
        assertNotNull(entry, "parcMemory_AllocateAndClear(%zu) returned NULL", sizeof(RtaConnectionEntry));
        entry->connection = connection;
        TAILQ_INSERT_TAIL(&table->head, entry, list);
        return 0;
    }
    return -1;
}

/**
 * Lookup a connection.
 * Returns NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnection *
rtaConnectionTable_GetByApiFd(RtaConnectionTable *table, int api_fd)
{
    assertNotNull(table, "Called with null parameter RtaConnectionTable");

    RtaConnectionEntry *entry;
    TAILQ_FOREACH(entry, &table->head, list)
    {
        if (rtaConnection_GetApiFd(entry->connection) == api_fd) {
            return entry->connection;
        }
    }
    return NULL;
}

/**
 * Lookup a connection.
 * Returns NULL if not found
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
RtaConnection *
rtaConnectionTable_GetByTransportFd(RtaConnectionTable *table, int transport_fd)
{
    assertNotNull(table, "Called with null parameter RtaConnectionTable");

    RtaConnectionEntry *entry;
    TAILQ_FOREACH(entry, &table->head, list)
    {
        if (rtaConnection_GetTransportFd(entry->connection) == transport_fd) {
            return entry->connection;
        }
    }
    return NULL;
}


/**
 * Remove a connection from the table, calling rtaConnection_Destroy() on it.
 * Returns 0 on success, -1 if not found (or error)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
rtaConnectionTable_Remove(RtaConnectionTable *table, RtaConnection *connection)
{
    assertNotNull(table, "Called with null parameter RtaConnectionTable");
    assertNotNull(connection, "Called with null parameter RtaConnection");

    RtaConnectionEntry *entry;
    TAILQ_FOREACH(entry, &table->head, list)
    {
        if (entry->connection == connection) {
            assertTrue(table->count_elements > 0, "Invalid state, found an entry, but count_elements is zero");
            table->count_elements--;
            TAILQ_REMOVE(&table->head, entry, list);
            if (table->freefunc) {
                table->freefunc(&entry->connection);
            }
            parcMemory_Deallocate((void **) &entry);
            return 0;
        }
    }
    return -1;
}

/**
 * Remove all connections in a given stack_id, calling rtaConnection_Destroy() on it.
 * Returns 0 on success, -1 if not found (or error)
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
int
rtaConnectionTable_RemoveByStack(RtaConnectionTable *table, int stack_id)
{
    assertNotNull(table, "Called with null parameter RtaConnectionTable");

    RtaConnectionEntry *entry = TAILQ_FIRST(&table->head);
    while (entry != NULL) {
        RtaConnectionEntry *temp = TAILQ_NEXT(entry, list);
        if (rtaConnection_GetStackId(entry->connection) == stack_id) {
            assertTrue(table->count_elements > 0, "Invalid state, found an entry, but count_elements is zero");
            table->count_elements--;

            if (DEBUG_OUTPUT) {
                printf("%9" PRIu64 "%s stack_id %d conn %p\n",
                       rtaFramework_GetTicks(rtaConnection_GetFramework(entry->connection)),
                       __func__,
                       stack_id,
                       (void *) entry->connection);
            }

            TAILQ_REMOVE(&table->head, entry, list);
            if (table->freefunc) {
                table->freefunc(&entry->connection);
            }

            if (DEBUG_OUTPUT) {
                printf("%9s %s FREEFUNC RETURNS\n",
                       " ", __func__);
            }

            parcMemory_Deallocate((void **) &entry);
        }
        entry = temp;
    }
    return 0;
}
