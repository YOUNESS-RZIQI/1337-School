#include "codexion.h"

void	pq_init(t_pqueue *pq)
{
	pq->size = 0;
}

int	pq_is_empty(t_pqueue *pq)
{
	return (pq->size == 0);
}

static void	pq_swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	pq_push(t_pqueue *pq, t_request req)
{
	int	i;
	int	parent;

	if (pq->size >= MAX_CODERS)
		return ;
	pq->data[pq->size] = req;
	i = pq->size;
	pq->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (pq->data[parent].sort_key <= pq->data[i].sort_key)
			break ;
		pq_swap(&pq->data[parent], &pq->data[i]);
		i = parent;
	}
}

static void	pq_sift_down(t_pqueue *pq)
{
	int	i;
	int	child;

	i = 0;
	while (i * 2 + 1 < pq->size)
	{
		child = i * 2 + 1;
		if (child + 1 < pq->size
			&& pq->data[child + 1].sort_key < pq->data[child].sort_key)
			child++;
		if (pq->data[i].sort_key <= pq->data[child].sort_key)
			break ;
		pq_swap(&pq->data[i], &pq->data[child]);
		i = child;
	}
}

t_request	pq_pop(t_pqueue *pq)
{
	t_request	top;

	top = pq->data[0];
	pq->size--;
	pq->data[0] = pq->data[pq->size];
	pq_sift_down(pq);
	return (top);
}
