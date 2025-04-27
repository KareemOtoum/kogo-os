#include "malloc.h"

static alloc::Block* head;
static alloc::Block* tail;

void create_heap()
{
    using namespace alloc;

    head = { reinterpret_cast<Block*>(k_HeapStart) };
    head->m_next = nullptr;
    head->m_size = k_HeapSize - sizeof(Block);
    tail = head;
}

size_t free_mem()
{
    using namespace alloc;
    size_t total{};
    
    Block* curr { head };
    while(curr)
    {
        total += curr->m_size;
        curr = curr->m_next;
    }

    return total;
}

void* malloc(size_t size)
{
    using namespace alloc;

    Block* curr { head };
    Block* prev { nullptr };
    const size_t real_size { size + sizeof(Header) };

    // first fit
    while(curr)
    {
        if(curr->m_size >= real_size) // found a fit
        {
            void* alloc_addr { curr };
            // move or dont move the free block
            // we only move if its larger than the amount we need
            if(curr->m_size > real_size)
            {
                // empty block and move it
                size_t remaining { curr->m_size - real_size };
                Block* next { curr->m_next };
                curr += real_size;
                curr->m_size = remaining;
                curr->m_next = next;

                // if a prev node points to this node relink it
                if(prev)
                {
                    prev->m_next = curr;
                }
                else // we nee to update head to point to this
                {
                    head = curr;
                }
            }
            else
            {
                // if the block will be removed link the prev to the next
                if(prev)
                {
                    prev->m_next = curr->m_next;
                }
                else // we removed the head
                {
                    head = curr->m_next;
                }

                // check if we removed the tail
                if(!curr->m_next)
                {
                    tail = prev;
                }
            }

            // place allocation header and return the reserved memory
            Header* alloc_header { reinterpret_cast<Header*>(alloc_addr) };
            alloc_header->m_size = size;
            alloc_header->m_magic = k_MagicNum;

            return alloc_addr + sizeof(Header);
        }
        prev = curr;
        curr = curr->m_next;
    }

    return nullptr;
}

void free(void* ptr)
{
    // make free block at the location
    // if tail is null set it as head and tail because that means free list is empty
    // if tail is not null set it to point to this new free block.
}