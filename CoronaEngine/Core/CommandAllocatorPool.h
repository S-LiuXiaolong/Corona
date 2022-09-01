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
    ����������ء�
    ���ڹ������������������
*/

#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <stdint.h>

class CommandAllocatorPool
{
public:
    // ������������ص�����
    CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE Type);
    ~CommandAllocatorPool();

    // ��ʼ�������������
    void Create(ID3D12Device* pDevice);
    // �ر������������
    void Shutdown();

    // ���ݵ�ǰ�Ѿ�ִ�����Χ��ֵ����ȡһ��������
    ID3D12CommandAllocator* RequestAllocator(uint64_t CompletedFenceValue);
    // ���÷�������Ӧ�������б��Ѿ���ExecuteCommandLists����ã���Ҫ���뵱ǰ�����б��Χ��ֵ
    void DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator* Allocator);

    inline size_t Size() { return m_AllocatorPool.size(); }

private:
    const D3D12_COMMAND_LIST_TYPE m_cCommandListType;

    ID3D12Device* m_Device;
    std::vector<ID3D12CommandAllocator*> m_AllocatorPool;
    std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> m_ReadyAllocators;
    std::mutex m_AllocatorMutex;
};
