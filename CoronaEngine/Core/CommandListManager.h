//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard
//

/*
    CommandQueue�����������
    CommandListManager: �����б������
*/

#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <stdint.h>
#include "CommandAllocatorPool.h"

class CommandQueue
{
    friend class CommandListManager;
    friend class CommandContext;

public:
    CommandQueue(D3D12_COMMAND_LIST_TYPE Type);
    ~CommandQueue();

    // �����������
    void Create(ID3D12Device* pDevice);
    void Shutdown();

    inline bool IsReady()
    {
        return m_CommandQueue != nullptr;
    }

    uint64_t IncrementFence(void);

    // ��Χ���Ƿ����
    bool IsFenceComplete(uint64_t FenceValue);
    // �ȴ�Χ����������Ҫȷ����ʱ��Χ���ض���û�н���
    void StallForFence(uint64_t FenceValue);
    // �ȴ����������ȫ�����ݽ�������Ҫȷ����ʱ��������бض���û�н���
    void StallForProducer(CommandQueue& Producer);
    // �ȴ�Χ������
    void WaitForFence(uint64_t FenceValue);
    // ��������������һ��Χ��ֵ���ȴ�����
    void WaitForIdle(void) { WaitForFence(IncrementFence()); }

    ID3D12CommandQueue* GetCommandQueue() { return m_CommandQueue; }

    uint64_t GetNextFenceValue() { return m_NextFenceValue; }

private:
    // �������б�����ݲ���gpu���������
    uint64_t ExecuteCommandList(ID3D12CommandList* List);
    // ����һ�����õĸ�������е����������
    ID3D12CommandAllocator* RequestAllocator(void);
    // ��һ���������ִ����ExecuteCommandList�󣬲���һ��Χ��ֵ�������ñ�������¼�����������key
    void DiscardAllocator(uint64_t FenceValueForReset, ID3D12CommandAllocator* Allocator);

    ID3D12CommandQueue* m_CommandQueue; // �������

    const D3D12_COMMAND_LIST_TYPE m_Type; // �������������

    CommandAllocatorPool m_AllocatorPool; // ����������أ�����m_Type
    std::mutex m_FenceMutex;
    std::mutex m_EventMutex;

    // Lifetime of these objects is managed by the descriptor cache
    ID3D12Fence* m_pFence;	// Χ�������Ի�ȡ�Ѿ�ִ�����Χ��ֵ�����жϵ�ǰ������ж�Ӧ������������Ƿ���Ը���
    uint64_t m_NextFenceValue;
    uint64_t m_LastCompletedFenceValue;
    HANDLE m_FenceEventHandle;

};

class CommandListManager
{
    friend class CommandContext;

public:
    CommandListManager();
    ~CommandListManager();

    void Create(ID3D12Device* pDevice);
    void Shutdown();

    CommandQueue& GetGraphicsQueue(void) { return m_GraphicsQueue; }
    CommandQueue& GetComputeQueue(void) { return m_ComputeQueue; }
    CommandQueue& GetCopyQueue(void) { return m_CopyQueue; }

    CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE Type = D3D12_COMMAND_LIST_TYPE_DIRECT)
    {
        switch (Type)
        {
        case D3D12_COMMAND_LIST_TYPE_COMPUTE: return m_ComputeQueue;
        case D3D12_COMMAND_LIST_TYPE_COPY: return m_CopyQueue;
        default: return m_GraphicsQueue;
        }
    }

    ID3D12CommandQueue* GetCommandQueue()
    {
        return m_GraphicsQueue.GetCommandQueue();
    }

    // �������ʹ���һ�������б��Լ���Ӧ�����������
    void CreateNewCommandList(
        D3D12_COMMAND_LIST_TYPE Type,
        ID3D12GraphicsCommandList** List,
        ID3D12CommandAllocator** Allocator);

    // Test to see if a fence has already been reached
    bool IsFenceComplete(uint64_t FenceValue)
    {
        return GetQueue(D3D12_COMMAND_LIST_TYPE(FenceValue >> 56)).IsFenceComplete(FenceValue);
    }

    // The CPU will wait for a fence to reach a specified value
    void WaitForFence(uint64_t FenceValue);

    // The CPU will wait for all command queues to empty (so that the GPU is idle)
    void IdleGPU(void)
    {
        m_GraphicsQueue.WaitForIdle();
        m_ComputeQueue.WaitForIdle();
        m_CopyQueue.WaitForIdle();
    }

private:

    ID3D12Device* m_Device;

    // Χ��3���������
    CommandQueue m_GraphicsQueue;
    CommandQueue m_ComputeQueue;
    CommandQueue m_CopyQueue;
};
