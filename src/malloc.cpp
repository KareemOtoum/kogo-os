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

void merge(alloc::Block* prev, alloc::Block* next)
{
    if(!prev || !next) return;

    using namespace alloc;

    prev->m_size += next->m_size + sizeof(Block);
    prev->m_next = next->m_next;

    // checking if the removed block is the tail
    if(!next->m_next)
    {
        tail = prev;
    }
}

// handles the entire process of coalescing when freeing a block
void coalesce(alloc::Block* block, alloc::Block* prev)
{
    if(!block) return;

    using namespace alloc;
    
    auto is_adjacent = [](Block* prev, Block* next)
    {
        return reinterpret_cast<char*>(prev) + prev->m_size + sizeof(Block) 
        == reinterpret_cast<char*>(next);
    };
    
    // try to merge with next block first
    if(block->m_next)
    {
        if(is_adjacent(block, block->m_next))
        {
            kout << "merging free block with block infront of it\n";
            merge(block, block->m_next);
        }
    }

    // merge with prev 
    if(prev)
    {
        if(is_adjacent(prev, block))
        {
            kout << "merging free blocks prev with it\n";
            merge(prev, block);
        }
    }
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

    Block* curr { head };
    Block* prev { nullptr };

    // make curr the free block after this one
    // and prev the one before it
    while (curr && curr < free_block)
    {
        prev = curr;
        curr = curr->m_next;
    }
    
    free_block->m_next = curr;

    if(prev)
    {
        prev->m_next = free_block;
    }
    else // its the first block
    {
        head = free_block;
    }

    if(!free_block->m_next) // its the tail
    {
        tail = free_block;
    }

    coalesce(free_block, prev);
}