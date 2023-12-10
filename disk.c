#include <stdlib.h>
#include "oslabs.h"
#define TRUE 1
#define FALSE 0

struct RCB createNullRCB();
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt);
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder);
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction);

struct RCB createNullRCB()
{
    struct RCB nullRCB;
    nullRCB.request_id = 0;
    nullRCB.arrival_timestamp = 0;
    nullRCB.cylinder = 0;
    nullRCB.address = 0;
    nullRCB.process_id = 0;
    return nullRCB;
};

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.process_id == 0)
    {
        return new_request;
    }

    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
};

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt)
{
    if (*queue_cnt == 0)
    {
        return createNullRCB();
    }
    int n1 = 0;
    int n2 = request_queue[0].arrival_timestamp;
    for (int x = 1; x < *queue_cnt; x++)
    {
        if (request_queue[x].arrival_timestamp < n2)
        {
            n1 = x;
            n2 = request_queue[x].arrival_timestamp;
        }
    }

    struct RCB next_rcb = request_queue[n1];
    for (int x = n1; x < (*queue_cnt) - 1; x++)
    {
        request_queue[x] = request_queue[x + 1];
    }
    (*queue_cnt)--;

    return next_rcb;
};
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.process_id == 0)
    {
        return new_request;
    }

    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
};
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder)
{
    if (*queue_cnt == 0)
    {
        return createNullRCB();
    }

    int next = 0;
    int next_cylinder_distance = abs(request_queue[0].cylinder - current_cylinder);
    int next_at = request_queue[0].arrival_timestamp;
    for (int x = 1; x < *queue_cnt; x++)
    {
        int current_cylinder_distance = abs(request_queue[x].cylinder - current_cylinder);
        if (current_cylinder_distance == next_cylinder_distance)
        {
            if (request_queue[x].arrival_timestamp < next_at)
            {
                next = x;
                next_cylinder_distance = current_cylinder_distance;
                next_at = request_queue[x].arrival_timestamp;
            }
        }
        else if (current_cylinder_distance < next_cylinder_distance)
        {
            next = x;
            next_cylinder_distance = current_cylinder_distance;
            next_at = request_queue[x].arrival_timestamp;
        }
    }

    struct RCB next_rcb = request_queue[next];
    for (int x = next; x < (*queue_cnt) - 1; x++)
    {
        request_queue[x] = request_queue[x + 1];
    }
    (*queue_cnt)--;

    return next_rcb;
};
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.process_id == 0)
    {
        return new_request;
    }

    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
};
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction)
{
    int n1 = -1;
    if (*queue_cnt == 0)
    {
        return createNullRCB();
    }

    if (scan_direction == 1)
    {
        n1 = find_next_rcb_up(request_queue, *queue_cnt, current_cylinder);
        if (n1 == -1)
        {
            n1 = find_next_rcb_down(request_queue, *queue_cnt, current_cylinder);
        }
    }
    else
    {
        n1 = find_next_rcb_down(request_queue, *queue_cnt, current_cylinder);
        if (n1 == -1)
        {
            n1 = find_next_rcb_up(request_queue, *queue_cnt, current_cylinder);
        }
    }

    struct RCB next_rcb = request_queue[n1];
    for (int x = n1; x < (*queue_cnt) - 1; x++)
    {
        request_queue[x] = request_queue[x + 1];
    }
    (*queue_cnt)--;
    return next_rcb;
};