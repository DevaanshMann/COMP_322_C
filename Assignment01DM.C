// Devaansh Mann
// Assignment 1 - Process Creation Hierarchy 
// COMP 322/L
// February 19, 2024


#include <stdio.h>
#include <stdlib.h>

// Define PCB structure
struct PCB {
    int parent;
    int first_child;
    int older_sibling;
    int younger_sibling;
};

// Define global variables
struct PCB *PCB_array;
int max_processes;

int find_last_child(int parent_index) {
    int last_child = PCB_array[parent_index].first_child;
    while (last_child != -1 && PCB_array[last_child].younger_sibling != -1) {
        last_child = PCB_array[last_child].younger_sibling;
    }
    return last_child;
}

// function to create child
void create_child(int parent_index) {
    int q;

    // Find the next unused PCB index q
    for (q = 0; q < max_processes && PCB_array[q].parent != -1; q++);

    if (q == max_processes) {
        printf("Error: No more processes can be created. PCB array is full.\n");
        return;
    }

    // Record the parent PCB index p in PCB[q]
    PCB_array[q].parent = parent_index;

    // Initialize the first_child, older_sibling, and younger_sibling fields of PCB[q]
    PCB_array[q].first_child = -1;
    PCB_array[q].older_sibling = -1;
    PCB_array[q].younger_sibling = -1;

    // Find the last child of the parent
    int last_child = find_last_child(parent_index);

    // If last_child is not NULL, update its younger sibling to q
    if (last_child != -1) {
        PCB_array[last_child].younger_sibling = q;
        PCB_array[q].older_sibling = last_child;
    } else {
        // q is the first child, update the parent's first child to q
        PCB_array[parent_index].first_child = q;
    }
}

// Function to destroy descendants
void destroy_descendants(int parent_index) {
    
    // Stack to simulate recursion
    int stack[max_processes];
    int highest = -1;

    // Push the initial parent index onto the stack
    stack[++highest] = parent_index;

    while (highest != -1) {
        int current_index = stack[highest--];

        // Call the recursive procedure on the first_child field of PCB[p]
        int q = PCB_array[current_index].first_child;
        while (q != -1) {
            stack[++highest] = q;
            q = PCB_array[q].younger_sibling;
        }

        // Update sibling pointers
        if (PCB_array[current_index].older_sibling != -1) {
            PCB_array[PCB_array[current_index].older_sibling].younger_sibling = -1;
        }
        if (PCB_array[current_index].parent != -1) {
            PCB_array[PCB_array[current_index].parent].first_child = -1;
        }

        // Set all fields of the current process to -1
        PCB_array[current_index].parent = -1;
        PCB_array[current_index].first_child = -1;
        PCB_array[current_index].older_sibling = -1;
        PCB_array[current_index].younger_sibling = -1;
    }
}

// Function to free memory
void freeMemory() {
    free(PCB_array);
}

// Function prototypes
void create_child(int parent_index);
void destroy_descendants(int parent_index);
void displayPCBTable();
void freeMemory();


// Function to print PCB table
void displayPCBTable() {
    printf("\ni     Parent     First     Older     Younger\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < max_processes; i++) {
        printf("%-2d      ", i);   //6 spaces

        // Print parent index or leave empty
        if (PCB_array[i].parent != -1) {
            printf("%-6d     ", PCB_array[i].parent);   //5 spaces
        } else {
            printf("         ");   // 9 spaces
        }

        // Print first_child index or leave empty
        if (PCB_array[i].first_child != -1) {
            printf("%-5d     ", PCB_array[i].first_child);   //5 spaces
        } else {
            printf("   ");   //3 spaces
        }

        // Print older_sibling index or leave empty
        if (PCB_array[i].older_sibling != -1) {
            printf("%-5d      ", PCB_array[i].older_sibling);   //6spaces
        } else {
            printf("                  ");   //18 spaces
        }

        // Print younger_sibling index or leave empty
        if (PCB_array[i].younger_sibling != -1) {
            printf("%-5d   ", PCB_array[i].younger_sibling);   //3spaces
        }

        printf("\n");
    }
}


int main() {
  
  

    int choice, parent_index;

    do {
        // Print the menu
        printf("\nProcess creation and destruction\n");
        printf("--------------------------------\n");
        printf("1) Enter parameters\n");
        printf("2) Create a new child process\n");
        printf("3) Destroy all descendants of a process\n");
        printf("4) Quit program and free memory\n");
        printf("Enter selection: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Prompt for the maximum number of processes
                printf("Enter maximum number of processes: ");
                scanf("%d", &max_processes);

                // Allocate memory for the dynamic array of PCBs
                PCB_array = (struct PCB*)malloc(max_processes * sizeof(struct PCB));
                if (PCB_array == NULL) {
                    printf("Error: Memory allocation failed.\n");
                    return 1;
                }

                // Initialize PCB_array                  (All initialized to -1 as it states that there is no value assigned to the block yet.)
                for (int i = 0; i < max_processes; i++) {
                    PCB_array[i].parent = -1;
                    PCB_array[i].first_child = -1;
                    PCB_array[i].older_sibling = -1;
                    PCB_array[i].younger_sibling = -1;
                }

                // Set the parent index of the first process (index 0) to 0
                PCB_array[0].parent = 0;

                // Print PCB table after each selection
                displayPCBTable();
                break;
            case 2:
                printf("Enter the parent process index: ");
                scanf("%d", &parent_index);
                create_child(parent_index);

                // Print PCB table after each selection
                displayPCBTable();
                break;
            case 3:
                printf("Enter the process whose descendants are to be destroyed: ");
                scanf("%d", &parent_index);
                destroy_descendants(parent_index);

                // Print PCB table after each selection
                displayPCBTable();
                break;
            case 4:
                freeMemory();
                printf("Quitting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                // Clear input buffer
                while (getchar() != '\n');
                break;
        }

    } while (choice != 4);                  // if choice = 4, terminate the program, else continue to show menu

    return 0;
}

