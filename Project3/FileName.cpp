//#include <stdio.h>
//#include <stdlib.h>
//
//// ���μ��� ��� ����ü ����
//typedef struct ProcessNode {
//    int data;
//    struct ProcessNode* next;
//} ProcessNode;
//
//// ���� ��� ����ü ����
//typedef struct StackNode {
//    ProcessNode* processList;
//    struct StackNode* next;
//} StackNode;
//
//// ���ο� ���μ��� ��带 �����ϰ� ����Ʈ�� �߰� (����Ʈ�� ���� ����)
//void insertProcess(ProcessNode** head, int data) {
//    ProcessNode* newNode = (ProcessNode*)malloc(sizeof(ProcessNode));
//    newNode->data = data;
//    newNode->next = NULL;
//
//    if (*head == NULL) {
//        *head = newNode;
//    }
//    else {
//        ProcessNode* current = *head;
//        while (current->next != NULL) {
//            current = current->next;
//        }
//        current->next = newNode;
//    }
//}
//
//// ���ο� ���� ��带 �����ϰ� ���ÿ� �߰�
//void push(StackNode** top, ProcessNode* processList) {
//    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
//    newNode->processList = processList;
//    newNode->next = *top;
//    *top = newNode;
//}
//
//// ������ �� �� ��带 �����ϰ� �ش� ����� ���μ��� ����Ʈ�� ��ȯ
//ProcessNode* pop(StackNode** top) {
//    if (*top == NULL) {
//        printf("������ ��� �ֽ��ϴ�.\n");
//        return NULL;
//    }
//    StackNode* temp = *top;
//    ProcessNode* processList = temp->processList;
//    *top = (*top)->next;
//    free(temp);
//    return processList;
//}
//
//// ���μ��� ����Ʈ ���
//void printProcessList(ProcessNode* head) {
//    ProcessNode* current = head;
//    while (current != NULL) {
//        printf("%d -> ", current->data);
//        current = current->next;
//    }
//    printf("NULL\n");
//}
//
//// ���� ���
//void printStack(StackNode* top) {
//    StackNode* current = top;
//    while (current != NULL) {
//        printf("���� ����� ���μ��� ����Ʈ: ");
//        printProcessList(current->processList);
//        current = current->next;
//    }
//}
//
//// ���μ��� ����Ʈ �޸� ����
//void freeProcessList(ProcessNode* head) {
//    ProcessNode* current = head;
//    while (current != NULL) {
//        ProcessNode* temp = current;
//        current = current->next;
//        free(temp);
//    }
//}
//
//// ���� �޸� ����
//void freeStack(StackNode* top) {
//    StackNode* current = top;
//    while (current != NULL) {
//        StackNode* temp = current;
//        freeProcessList(current->processList);
//        current = current->next;
//        free(temp);
//    }
//}
//
//void dispatchProcess(StackNode** top) {
//    if (*top == NULL) {
//        printf("������ ��� �ֽ��ϴ�.\n");
//        return;
//    }
//    // top ���� ����� ���μ��� ����Ʈ�� ù ��° ��带 ������
//    ProcessNode* head = (*top)->processList;
//    if (head == NULL) {
//        printf("���μ��� ����Ʈ�� ��� �ֽ��ϴ�.\n");
//        return;
//    }
//    // ù ��° ��� ����
//    ProcessNode* temp = head;
//    (*top)->processList = head->next;
//    free(temp);
//
//    // ���� �� ����Ʈ�� ��� ������ ���ÿ��� �ش� ��带 ����
//    if ((*top)->processList == NULL) {
//        StackNode* tempStackNode = *top;
//        *top = (*top)->next;
//        free(tempStackNode);
//    }
//}
//
//void promote(StackNode** top, StackNode* P) {
//    if (P == NULL || *top == NULL || P->processList == NULL) {
//        printf("Promotion�� ������ �� �����ϴ�.\n");
//        return;
//    }
//
//    // P�� ���μ��� ����Ʈ���� ù ��° ��带 �����ϰ� ����
//    ProcessNode* nodeToPromote = P->processList;
//    P->processList = P->processList->next;
//    nodeToPromote->next = NULL; // �̵��� ���� �ܵ� ��尡 ��
//
//    // P�� ������ �� �� �����, ������ �� �Ʒ��� �߰�
//    if (P->next == NULL) {
//        StackNode* current = *top;
//        while (current->next != NULL) {
//            current = current->next;
//        }
//        // �� ���� ��� ���� �� ����
//        StackNode* newStackNode = (StackNode*)malloc(sizeof(StackNode));
//        newStackNode->processList = nodeToPromote;
//        newStackNode->next = NULL;
//        current->next = newStackNode;
//    }
//    else {
//        // P ���� ���� ����� ���μ��� ����Ʈ�� ���� �߰�
//        StackNode* nextStackNode = P->next;
//        ProcessNode* currentProcess = nextStackNode->processList;
//        if (currentProcess == NULL) {
//            // ���� ���� ����� ���μ��� ����Ʈ�� ��� �ִٸ� �ٷ� ����
//            nextStackNode->processList = nodeToPromote;
//        }
//        else {
//            // ���μ��� ����Ʈ�� ���� ã�� ����
//            while (currentProcess->next != NULL) {
//                currentProcess = currentProcess->next;
//            }
//            currentProcess->next = nodeToPromote;
//        }
//    }
//
//    // ���� ����Ʈ�� ��� ������ ���� ��带 ����
//    if (P->processList == NULL) {
//        // P�� top�̸� Ư�� ó��
//        if (P == *top) {
//            *top = P->next;
//        }
//        else {
//            // P�� ���� ��� ã��
//            StackNode* prev = *top;
//            while (prev != NULL && prev->next != P) {
//                prev = prev->next;
//            }
//            if (prev != NULL) {
//                prev->next = P->next;
//            }
//        }
//        free(P);
//    }
//}
//
//// ���μ��� ����Ʈ�� ���̸� ��ȯ�ϴ� �Լ�
//int processListLength(ProcessNode* head) {
//    int count = 0;
//    ProcessNode* current = head;
//    while (current != NULL) {
//        count++;
//        current = current->next;
//    }
//    return count;
//}
//
//// ������ ��� ���� ��ȯ�ϴ� �Լ�
//int stackNodeCount(StackNode* top) {
//    int count = 0;
//    StackNode* current = top;
//    while (current != NULL) {
//        count++;
//        current = current->next;
//    }
//    return count;
//}
//
//// ��ü ���μ����� ������ ��ȯ�ϴ� �Լ�
//int totalProcessCount(StackNode* top) {
//    int total = 0;
//    StackNode* current = top;
//    while (current != NULL) {
//        total += processListLength(current->processList);
//        current = current->next;
//    }
//    return total;
//}
//
//// split_n_merge �Լ� ����
//void split_n_merge(StackNode** top) {
//    if (*top == NULL) return;
//
//    int totalProcesses = totalProcessCount(*top);
//    int stackNodes = stackNodeCount(*top);
//    int threshold = totalProcesses / stackNodes;
//
//    StackNode* currentStackNode = *top;
//    while (currentStackNode != NULL) {
//        int listLength = processListLength(currentStackNode->processList);
//        // ����Ʈ ���̰� threshold���� ū ��� ���� �� ���� ����
//        if (listLength > threshold) {
//            ProcessNode* currentProcess = currentStackNode->processList;
//            ProcessNode* midPrevProcess = NULL;
//            // �߰� ���� ã��
//            for (int i = 0; i < 
//                / 2 - 1; i++) {
//                currentProcess = currentProcess->next;
//            }
//            midPrevProcess = currentProcess;
//            currentProcess = currentProcess->next; // �߰� ������ ���
//            midPrevProcess->next = NULL; // ���� ���� �и�
//
//            // ���� ����Ʈ�� ����
//            if (currentStackNode->next == NULL) { // �ֻ��� ����Ʈ�� ���
//                push(top, currentProcess); // �� ���� ���� �߰�
//            }
//            else {
//                // ���� ����Ʈ�� ���� ����
//                ProcessNode* tail = currentStackNode->next->processList;
//                if (tail == NULL) {
//                    currentStackNode->next->processList = currentProcess;
//                }
//                else {
//                    while (tail->next != NULL) {
//                        tail = tail->next;
//                    }
//                    tail->next = currentProcess;
//                }
//            }
//            // ��������� threshold �˻� �� ���� ���� ����
//            split_n_merge(top);
//            return; // ���� �Ŀ��� �� �̻� ���� �������� �������� ����
//        }
//        currentStackNode = currentStackNode->next;
//    }
//}
//
//// ���� �Լ�
//int main() {
//    StackNode* stack = NULL;
//
//    // ù ��° ���� ��忡 ���μ��� ����Ʈ �߰�
//    ProcessNode* processList1 = NULL;
//    insertProcess(&processList1, 1);
//    insertProcess(&processList1, 2);
//    insertProcess(&processList1, 3);
//    push(&stack, processList1);
//
//    // �� ��° ���� ��忡 ���μ��� ����Ʈ �߰�
//    ProcessNode* processList2 = NULL;
//    insertProcess(&processList2, 4);
//    insertProcess(&processList2, 5);
//    push(&stack, processList2);
//
//    // �� ��° ���� ��忡 ���μ��� ����Ʈ �߰�
//    ProcessNode* processList3 = NULL;
//    insertProcess(&processList3, 6);
//    insertProcess(&processList3, 7);
//    push(&stack, processList3);
//
//    // ���� ���
//    printf("�ʱ� ���� ����:\n");
//    printStack(stack);
//
//    // dispatchProcess �Լ� ȣ��
//    printf("\nDispatch ���μ��� ���� ��:\n");
//    dispatchProcess(&stack);
//    printStack(stack);
//
//    // promote �Լ� ȣ�� ����
//    // ���⼭�� P�� �� ��° ���� ���� �����Ͽ� promote�� �����մϴ�.
//    printf("\nPromote ���� ��:\n");
//    promote(&stack, stack->next); // stack->next�� �� ��° ��带 ����ŵ�ϴ�.
//    printStack(stack);
//
//    // �޸� ����
//    freeStack(stack);
//
//    return 0;
//}