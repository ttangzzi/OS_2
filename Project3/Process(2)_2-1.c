//#include <stdio.h>
//#include <stdlib.h>
//
///* 프로세스 노드 구조체 */
//typedef struct ProcessNode {
//    int data;
//    struct ProcessNode* next;
//} ProcessNode;
//
///* 스택 노드 구조체 */
//typedef struct StackNode {
//    ProcessNode* processList;
//    struct StackNode* next;
//} StackNode;
//
///* 1. enqueue 구현하기 */
//void enqueue(ProcessNode** head, int data) {
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
///* 새로운 스택 노드를 생성하고 스택에 추가 */
//void push(StackNode** top, ProcessNode* processList) {
//    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
//    newNode->processList = processList;
//    newNode->next = *top;
//    *top = newNode;
//}
//
///* 스택의 맨 위 노드를 제거 */
//ProcessNode* pop(StackNode** top) {
//    if (*top == NULL) {
//        printf("Stack is empty.\n");
//        return NULL;
//    }
//    StackNode* temp = *top;
//    ProcessNode* processList = temp->processList;
//    *top = (*top)->next;
//    free(temp);
//    return processList; // 프로세스 리스트 return
//}
//
///* 프로세스 리스트 출력 */ 
//void printProcessList(ProcessNode* head) {
//    ProcessNode* current = head;
//    while (current != NULL) {
//        printf("%d -> ", current->data);
//        current = current->next;
//    }
//    printf("NULL\n");
//}
//
///* 스택 출력 */
//void printStack(StackNode* top) {
//    StackNode* current = top;
//    while (current != NULL) {
//        printf("List of Stack Node: ");
//        printProcessList(current->processList);
//        current = current->next;
//    }
//}
//
///* 프로세스 리스트 메모리 해제 */
//void freeProcessList(ProcessNode* head) {
//    ProcessNode* current = head;
//    while (current != NULL) {
//        ProcessNode* temp = current;
//        current = current->next;
//        free(temp);
//    }
//}
//
///* 스택 메모리 해제 */
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
///* 2. dequeue 구현하기 */
//void dequeue(StackNode** top) {
//    if (*top == NULL) {
//        printf("Stack is empty.\n");
//        return;
//    }
//    // top 스택 노드의 프로세스 리스트의 첫 번째 노드
//    ProcessNode* head = (*top)->processList;
//    if (head == NULL) {
//        printf("Process List is empty.\n");
//        return;
//    }
//    // 첫 번째 노드 삭제
//    ProcessNode* temp = head;
//    (*top)->processList = head->next;
//    free(temp);
//
//    // 삭제 후 리스트가 비어 있으면 스택에서 해당 노드를 제거
//    if ((*top)->processList == NULL) {
//        StackNode* tempStackNode = *top;
//        *top = (*top)->next;
//        free(tempStackNode);
//    }
//}
//
///* 3. promote 구현하기 */
//void promote(StackNode** top, StackNode* P) {
//    if (P == NULL || *top == NULL || P->processList == NULL) {
//        printf("Impossible Promote.\n");
//        return;
//    }
//
//    // P의 프로세스 리스트에서 첫 번째 노드를 제거하고 저장
//    ProcessNode* nodeToPromote = P->processList;
//    P->processList = P->processList->next;
//    nodeToPromote->next = NULL; // 이동될 노드는 단독 노드
//
//    // P가 스택의 맨 위 노드라면, 스택의 맨 아래에 추가
//    if (P->next == NULL) {
//        StackNode* current = *top;
//        while (current->next != NULL) {
//            current = current->next;
//        }
//        // 새 스택 노드 생성 및 연결
//        StackNode* newStackNode = (StackNode*)malloc(sizeof(StackNode));
//        newStackNode->processList = nodeToPromote;
//        newStackNode->next = NULL;
//        current->next = newStackNode;
//    }
//    else {
//        // P 다음 스택 노드의 프로세스 리스트의 끝에 추가
//        StackNode* nextStackNode = P->next;
//        ProcessNode* currentProcess = nextStackNode->processList;
//        if (currentProcess == NULL) {
//            // 다음 스택 노드의 프로세스 리스트가 비어 있다면 바로 연결
//            nextStackNode->processList = nodeToPromote;
//        }
//        else {
//            // 프로세스 리스트의 끝을 찾아 연결
//            while (currentProcess->next != NULL) {
//                currentProcess = currentProcess->next;
//            }
//            currentProcess->next = nodeToPromote;
//        }
//    }
//
//    // 원래 리스트가 비어 있으면 스택 노드를 제거
//    if (P->processList == NULL) {
//        // P가 top이면 특별 처리
//        if (P == *top) {
//            *top = P->next;
//        }
//        else {
//            // P의 이전 노드 찾기
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
///* 프로세스 리스트의 길이 */
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
///* 스택의 노드 수 */
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
//
//int main() {
//    StackNode* stack = NULL;
//
//    // 첫 번째 스택 노드에 프로세스 리스트 추가
//    ProcessNode* processList1 = NULL;
//    enqueue(&processList1, 1);
//    enqueue(&processList1, 2);
//    enqueue(&processList1, 3);
//    push(&stack, processList1);
//
//    // 두 번째 스택 노드에 프로세스 리스트 추가
//    ProcessNode* processList2 = NULL;
//    enqueue(&processList2, 4);
//    enqueue(&processList2, 5);
//    push(&stack, processList2);
//
//    // 세 번째 스택 노드에 프로세스 리스트 추가
//    ProcessNode* processList3 = NULL;
//    enqueue(&processList3, 6);
//    enqueue(&processList3, 7);
//    push(&stack, processList3);
//
//    // 초기 스택 출력
//    printf("First stack state: \n");
//    printStack(stack);
//
//    // dequeue 함수 호출
//    printf("\nAfter running dequeue : \n");
//    dequeue(&stack);
//    printStack(stack);
//
//    // promote 함수 호출 예시
//    // P를 두 번째 스택 노드로 설정
//    printf("\nAfter running Promote: \n");
//    promote(&stack, stack->next);
//    printStack(stack);
//
//    
//    freeStack(stack);
//
//    return 0;
//}
