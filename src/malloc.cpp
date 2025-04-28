#include "malloc.h"

static alloc::Block* head;
static alloc::Block* tail;

void create_heap()
{
    using namespace alloc;

    head = reinterpret_cast<Block*>(k_HeapStart);
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

void log_freelist()
{
    using namespace alloc;
    
    Block* curr { head };
    while(curr)
    {
        kout << "Free Block at: " << curr  << " -> " << static_cast<int>(curr->m_size) 
            << " next -> " << curr->m_next << "\n";
        curr = curr->m_next;
    }
}

void scan_heap()
{
    using namespace alloc;
    uint8_t* heap = reinterpret_cast<uint8_t*>(k_HeapStart);
    uint8_t* end  = heap + k_HeapSize;

    while (heap < end)
    {
        Header* header = reinterpret_cast<Header*>(heap);
        
        // Check if it looks like a valid allocation
        if (header->m_magic == k_MagicNum)
        {
            kout << "[ALLOCATED] at " << header 
                 << " size: " << static_cast<int>(header->m_size) << "\n";
            
            size_t total_alloc_size = header->m_size + sizeof(Header);
            heap += total_alloc_size;
        }
        else
        {
            Block* block = reinterpret_cast<Block*>(heap);

            kout << "[FREE BLOCK] at " << block 
                 << " size: " << static_cast<int>(block->m_size) << "\n";
            
            size_t total_free_size = block->m_size + sizeof(Block);
            heap += total_free_size;
        }
    }
}


void* malloc(size_t size)
{
    using namespace alloc;

    Block* curr { head };
    Block* prev { nullptr };
    size_t real_size { size + sizeof(Header) };

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
                // move block to new position
                size_t remaining { curr->m_size - real_size };
                Block* next { curr->m_next };
                curr = reinterpret_cast<Block*>(reinterpret_cast<char*>(curr) + real_size);
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
                
                // relink if its the tail
                if(!curr->m_next)
                {
                    tail = curr;
                }
            }
            else
            {
                // free block will be removed so add size of block 
                real_size += sizeof(Header);
                
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
            alloc_header->m_size = real_size;
            alloc_header->m_magic = k_MagicNum;

            return reinterpret_cast<char*>(alloc_addr) + sizeof(Header);
        }
        prev = curr;
        curr = curr->m_next;
    }

    return nullptr;
}

void free(void* ptr)
{
    using namespace alloc;

    if(!ptr)
    {
        kout << "free error: invalid pointer\n";
        return;
    }
    // make free block at the location
    ptr = reinterpret_cast<char*>(ptr) - sizeof(Header);

    Header* header = reinterpret_cast<Header*>(ptr);
    if(header->m_magic != k_MagicNum) 
    {
        kout << "freeing error: magic number does not match\n";
        return;
    }
    size_t freed_size { header->m_size };
    
    Block* free_block { reinterpret_cast<Block*>(header) };
    free_block->m_size = freed_size - sizeof(Block);
    free_block->m_next = nullptr;

    // if head is nullptr set it as head and tail because that means free list is empty
    if(!head)
    {
        head = free_block;
        tail = head;
    }
    else // if head is not null set tail to point to this new free block.
    {
        tail->m_next = free_block;
        tail = free_block;
    }
}