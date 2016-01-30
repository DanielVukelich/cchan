#pragma once

/* send a whole static file to a client
 * Return 200 on success, 404 if file doesn't exist,
 * 401 if permissions are missing (not implemented yet),
 * 503 on error (should not happen) */
int send_static(int socket, char filename[]);

