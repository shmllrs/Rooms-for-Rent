/*
    Name: Shamel Larosa
    Student #: 2023-06239
    Project #1: Rooms for Rent
    Date: June 5, 2024
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//structure for tenants
typedef struct tenants {
    char first_name[50];
    char last_name[50];
    char mobile_number[12];
    struct tenants *next_tenants;
} tenants;

//structure for units
typedef struct units {
    char unit_name[50];
    int max_occupants; 
    int current_occupants; //to track current number of occupants per unit
    struct tenants *uns_tenants; //head tenants
    struct units *next_units;
} units;


//function for adding a unit
void add_unit(struct units **uns) {
    //memory allocation of new unit
    struct units *new_unit = (struct units *) malloc (sizeof(struct units));

    //ask user for new unit name
    printf("Enter unit name: ");
    //new unit name will be pass down to the structure with member name (unit_name)
    scanf(" %[^\n]", new_unit->unit_name);

    //initialize pointer current to the list of units
    units *current = *uns;
    //initialize pointer prev to null
    units *prev = NULL;

    //check if the current unit name (inputted by the user) is the same with any of the stored unit names
    while (current != NULL && strcasecmp(current->unit_name, new_unit->unit_name) < 0) {
        //initialize prev to current
        prev = current;
        //initialize current to next node which is to the next unit 
        current = current->next_units;
    }

    //if found the same name then prints error message
    if (current != NULL && strcasecmp(current->unit_name, new_unit->unit_name) == 0) {
        printf("\nUnit name already exists!\n");
        //frees the entered unit name which has a duplicate name to avoid memory leak
        free(new_unit);
        return;
    }

    //ask user for max occupants
    printf("Enter maximum occupants: ");
    //new unit's max occupant will be pass down to the structure with member name (max_occupants)
    scanf("%d", &new_unit->max_occupants);

    //initialize current occupants to 0
    new_unit->current_occupants = 0;
    //initialize head tenants to empty
    new_unit->uns_tenants = NULL;

    //inserts the new unit at beginning if empty
    if (prev == NULL) {
        //initialize next unit to head
        new_unit->next_units = *uns;
        //initialize head to new unit
        *uns = new_unit;
    } 
    //else inserts the new unit to next
    else {
        //initialize next unit node with the next unit
        new_unit->next_units = prev->next_units;
        //initalize the next unit to the entered new unit
        prev->next_units = new_unit;
    }

    //prints success message
    printf("\nUnit %s added successfully!\n", new_unit->unit_name);
}

//function for adding a tenant to a unit
void add_tenant(struct units *uns, struct tenants **tens) {
    //if there are no units added yet, prints an error message
    if (uns == NULL) {
        printf("There are no units available!\n");
        return;
    }

    //declare a variable to store units that are occupied
    char units_occupied[50];
    //memory allocation for new tenant
    struct tenants *new_tenant = (tenants *) malloc (sizeof(tenants));

    //ask user for first name
    printf("Enter first name: ");
    //first name will be pass down to the structure member first name
    scanf(" %[^\n]", new_tenant->first_name);

    //ask user for last name
    printf("Enter last name: ");
    //last name will be pass down to the structure member last name
    scanf(" %[^\n]", new_tenant->last_name);

    //pointer to traverse through the list of units
    struct units *current_unit = uns;

    //loop while the current unit is not empty
    //check if the tenant already exists in any unit (case insensitive this time)
    while (current_unit != NULL) {
        //initialize pointer current_tenant to the list of tenants
        struct tenants *current_tenant = current_unit->uns_tenants;
        //loop that traverse through the list of tenants in the current unit
        while (current_tenant != NULL) {
            //comparison of first and last name regardless ng cases
            if (strcasecmp(current_tenant->first_name, new_tenant->first_name) == 0 &&
                strcasecmp(current_tenant->last_name, new_tenant->last_name) == 0) {
                printf("\nError: Tenant can only occupy one unit at the same time!\n");
                //frees allocated memory if there is duplication to avoid memory leak
                free(new_tenant);
                return;
            }
            //move to next tenant
            current_tenant = current_tenant->next_tenants;
        }
        //move to next unit
        current_unit = current_unit->next_units;
    }

    //ask user for mobile number
    printf("Enter mobile number (11 digit format ie. 09123456789): ");
    //mobile number will pass down to structure member named mobile number
    scanf("%s", new_tenant->mobile_number);

    //shows units with occupants real time
    printf("\n===== UNITS ======\n");
    //initialize current unit to head
    current_unit = uns;
    //traverse thru the list while the current unit is not empty
    while (current_unit != NULL) {
        //prints the unit with the occupants
        printf("- %s (Occupants: %d/%d)\n", current_unit->unit_name, current_unit->current_occupants, current_unit->max_occupants);
        //move to next unit
        current_unit = current_unit->next_units;
    }
    printf("============================\n");

    //ask user for unit name to occupy
    printf("Enter unit name to occupy: ");
    //unit name will be stored in units occupied variable
    scanf(" %[^\n]", units_occupied);

    //initiaize current unit to head units
    current_unit = uns;

    //loop while current_unit is not empty
    while (current_unit != NULL) {
        //check if the current unit matches the provided unit name regardless of its cases
        if (strcasecmp(current_unit->unit_name, units_occupied) == 0) { 
            //check if the unit is already full
            if (current_unit->current_occupants >= current_unit->max_occupants) {
                //prints error message if unit has reached its max occupants
                printf("\nError: Unit %s is already full!\n", units_occupied); 
                //frees allocated memory to avoid memory leak
                free(new_tenant);
                return;
            }

            //initialize prev pointed to empty
            struct tenants *prev = NULL;
            //initialize current tenant to the head tenant
            struct tenants *current_tenant = current_unit->uns_tenants;

            //inserts new tenant based on their alphabetical order thru last name regardless of its cases
            while (current_tenant != NULL && (strcasecmp(current_tenant->last_name, new_tenant->last_name) < 0 || 
                   (strcasecmp(current_tenant->last_name, new_tenant->last_name) == 0 && strcasecmp(current_tenant->first_name, new_tenant->first_name) < 0))) {
                //initialize prev to current tenant
                prev = current_tenant;
                //initialize current tenant to the next tenant
                current_tenant = current_tenant->next_tenants;
            }

            //if there's no tenant then inserts at the head (beginning)
            if (prev == NULL) {
                //link new tenant to the current first tenant
                new_tenant->next_tenants = current_unit->uns_tenants;
                //initialize the new tenant as the first tenant of the unit
                current_unit->uns_tenants = new_tenant;
            } 
            //if there is then inserts at the next
            else {
                //link new tenant to the next tenant
                new_tenant->next_tenants = prev->next_tenants;
                //initialize previous tenant to the new tenant
                prev->next_tenants = new_tenant;
            }

            //increment the number of current occupants at the unit
            current_unit->current_occupants++;
            printf("\nSuccessfully occupied unit %s!\n", units_occupied);
            //updates head tenant with new tenant
            *tens = new_tenant;
            return;
        }
        //move to next unit
        current_unit = current_unit->next_units;
    }

    //prints error message if unit is not found
    printf("\nError: Unit %s not found!\n", units_occupied);
}

//function for searching a specific tenant
void search_tenant(struct units *uns, struct tenants *tens) {
    //if there are no tenants added yet, print an error message
    if (tens == NULL) {
        printf("There are no tenants as of the moment!\n");
        return;
    }

    //declare variable for first and last name
    char first_name[50];
    char last_name[50];

    //ask for first name
    printf("Enter first name: ");
    scanf(" %[^\n]", first_name);

    //ask for first name
    printf("Enter last name: ");
    scanf(" %[^\n]", last_name);

    //pointer to traverse through the list of units
    struct units *current_unit = uns;

    //declare and initialize if tenant is found
    int found = 0;

    //traverse thru each unit
    while (current_unit != NULL) {
        //initialize current tenant with the list of tenants
        struct tenants *current_tenant = current_unit->uns_tenants;
        while (current_tenant != NULL) {
            //comparison of first and last names regardless if cases
            if (strcasecmp(current_tenant->first_name, first_name) == 0 &&
                strcasecmp(current_tenant->last_name, last_name) == 0) {
                //tenant found, print their details
                printf("\n====== TENANT'S INFORMATION ======\n");
                printf("Unit: %s\n", current_unit->unit_name);
                printf("First Name: %s\n", current_tenant->first_name);
                printf("Last Name: %s\n", current_tenant->last_name);
                printf("Mobile Number: %s\n", current_tenant->mobile_number);
                //if tenant is found
                found = 1;
                return;
            }
            //initialize current tenant to the next node which is next tenant
            current_tenant = current_tenant->next_tenants;
        }
        //move to the next unit if tenant is not found in the current unit
        current_unit = current_unit->next_units;
    }

    //if tenant is not found in any unit prints error message
    if (found == 0) {
        printf("\nError: Tenant not found!\n");
    }
}

//function for moving a tenant to another unit
void move_tenant(struct units *uns, struct tenants *tens) {
    //if tenant list is empty, prints error message
    if (tens == NULL) {
        printf("There are no tenants as of the moment!\n");
        return;
    }

    //initialize current unit to the list of units
    struct units *current_unit = uns;
    //declare pointer current tenant
    struct tenants *current_tenant;

    //declare variables
    char first_name[50];
    char last_name[50];

    //ask for first name
    printf("Enter first name: ");
    scanf(" %[^\n]", first_name);

    //ask for last name
    printf("Enter last name: ");
    scanf(" %[^\n]", last_name);

    //declare and initialize 0 if tenant is found
    int tenant_found = 0;

    //traverse thru the list of current unit if its not empty
    while (current_unit != NULL) {
        //initialize current tenant with the list of tenants
        current_tenant = current_unit->uns_tenants;
        //initialize pointer prev of tenant to empty
        struct tenants *prev_tenant = NULL;
        //traverse thru the list of current tenant if its not empty
        while (current_tenant != NULL) {
            //comparison of first and last name regardless of cases
            if (strcasecmp(current_tenant->first_name, first_name) == 0 &&
                strcasecmp(current_tenant->last_name, last_name) == 0) {
                //declare and initalize if tenant is found
                tenant_found = 1;
                //decrement current occupants count for the entered unit
                current_unit->current_occupants--;
                //remove the tenant from the current unit's tenant list
                if (prev_tenant == NULL) {
                    //initialize the head of tenants to the next tenant if the list is empty
                    current_unit->uns_tenants = current_tenant->next_tenants;
                } 
                else {
                    //initialize prev tenant to the next tenant if the list has data
                    prev_tenant->next_tenants = current_tenant->next_tenants;
                }

                //shows units with occupants real time
                printf("\n===== UNITS ======\n");
                //initialize temp pointer to list of units
                struct units *temp = uns;
                //traverse thru the list of units if the the list is not empty
                while (temp != NULL) {
                    //prints the units and occupants
                    printf("- %s (Occupants: %d/%d)\n", temp->unit_name, temp->current_occupants, temp->max_occupants);
                    //initialize temp to the next node which is next units
                    temp = temp->next_units;
                }
                printf("============================\n");

                //declare a variable
                char selected_unit[50];

                //ask user to the unit name to move in
                printf("Enter the unit name where the tenant would move in: ");
                scanf(" %[^\n]", selected_unit);

                //initialize destionation unit to the list of units
                struct units *dest_unit = uns;

                //traverse thru the list of units if its not empty
                while (dest_unit != NULL) {
                    if (strcasecmp(dest_unit->unit_name, selected_unit) == 0) {
                        //check if the destination unit is already at its maximum occupancy
                        if (dest_unit->current_occupants >= dest_unit->max_occupants) {
                            printf("\nError: Unit %s is already full!\n", selected_unit);
                            //restore the tenant to the entered unit
                            current_unit->current_occupants++;
                            //if the prev tenant is empty then it would be assigned to the current tenant entered
                            if (prev_tenant == NULL) {
                                current_unit->uns_tenants = current_tenant;
                            } 
                            //else if it has a data stored then it would be assigned to the next tenant with the current tenant entered
                            else {
                                prev_tenant->next_tenants = current_tenant;
                            }
                            return;
                        }

                        //insert the tenant into the destination unit in alphabetical order
                        //memory allocation of new tenant
                        struct tenants *new_tenant = (struct tenants *)malloc(sizeof(struct tenants));
                        //if the list is empty prints an error message
                        if (new_tenant == NULL) {
                            printf("Sorry, there are no tenants as of the moment!\n");
                            //restore the tenant to the source unit
                            current_unit->current_occupants++;
                            //if prev tenant is empty then it would be assigned to the current tenant entered
                            if (prev_tenant == NULL) {
                                current_unit->uns_tenants = current_tenant;
                            } 
                            //else if it has a data stored then it would be assigned to the next tenant with the current tenant entered
                            else {
                                prev_tenant->next_tenants = current_tenant;
                            }
                            return;
                        }
                        //copies the string of data of the new tenant to the structure 
                        strcpy(new_tenant->first_name, first_name);
                        strcpy(new_tenant->last_name, last_name);
                        strcpy(new_tenant->mobile_number, current_tenant->mobile_number);

                        //initialize prev pointer to empty
                        struct tenants *prev = NULL;
                        //initialize current pointer to the list of tenants
                        struct tenants *curr = dest_unit->uns_tenants;

                        //traverse thru the list of current tenants regardless of its cases
                        while (curr != NULL && (strcasecmp(curr->last_name, new_tenant->last_name) < 0 ||
                            (strcasecmp(curr->last_name, new_tenant->last_name) == 0 && strcasecmp(curr->first_name, new_tenant->first_name) < 0))) {
                            //assign prev tenant to current tenant
                            prev = curr;
                            //assign current tenant to the next tenant
                            curr = curr->next_tenants;
                        }

                        //if the node prev is empty
                        if (prev == NULL) {
                            //assigns the new tenant to the beginning of the list of tenants associated with the destination unit
                            new_tenant->next_tenants = dest_unit->uns_tenants;
                            //sets the new tenant as the head of the list of tenants for the destination unit
                            dest_unit->uns_tenants = new_tenant;
                        } 
                        else {
                            //inserts new tenant after the previous tenant
                            new_tenant->next_tenants = prev->next_tenants;
                            //updates next tenant to new tenant
                            prev->next_tenants = new_tenant;
                        }
                        //increment the number of current occupants in the destination unit
                        dest_unit->current_occupants++;
                        //prints successful message if moved
                        printf("\nSuccessfully moved to unit %s!\n", selected_unit);
                        //frees the current tenant
                        free(current_tenant);
                        return;
                    }
                    //assign destination unit to the next unit
                    dest_unit = dest_unit->next_units;
                }

                //prints error message
                printf("\nError: Unit not found!\n");
                //restore the tenant to the source unit
                current_unit->current_occupants++;

                //if the prev tenant is empty then it would be assigned to the current tenant entered
                if (prev_tenant == NULL) {
                    current_unit->uns_tenants = current_tenant;
                } 
                //else if it has a data stored then it would be assigned to the next tenant with the current tenant entered
                else {
                    prev_tenant->next_tenants = current_tenant;
                }
                return;
            }
            //update prev tenant to current tenant
            prev_tenant = current_tenant;
            //update current tenant to the next node (next tenant)
            current_tenant = current_tenant->next_tenants;
        }
        //if the tenant is found breaks
        if (tenant_found) break;
        //sets current unit to the next unit
        current_unit = current_unit->next_units;
    }

    //if tenant is not found then prints an error message
    if (tenant_found == 0) {
        printf("\nError: Tenant not found!\n");
        return;
    }
}

//function for removing tenant
void remove_tenant(struct units **uns, struct tenants **tens){
    //check if there are any tenants, prints error message if there's not
    if (tens == NULL){
        printf("There are no tenants as of the moment!\n");
        return;
    }

    //initialize current unit pointer to the list of units
    struct units *current_unit = *uns;
    //initialize prev tenant pointer to empty
    struct tenants *previous_tenant = NULL;
    //initiallize current tenant
    struct tenants *current_tenant;

    //declare variables
    char first_name[50];
    char last_name[50];

    //ask for first name
    printf("Enter first name: ");
    scanf(" %[^\n]", first_name);

    //ask for last name
    printf("Enter last name: ");
    scanf(" %[^\n]", last_name);

    //declare variable and initialize if tenant is found
    int tenant_found = 0;

    //traverse thru list of units if its not empty
    while (current_unit != NULL) {
        //set current tenant to the list of tenants
        current_tenant = current_unit->uns_tenants;
        //assign prev tenant to empty
        previous_tenant = NULL;
        //traverse thru the list of tenants if its not empty
        while (current_tenant != NULL) {
            //checks for the names regardless of its cases
            if (strcasecmp(current_tenant->first_name, first_name) == 0 &&
                strcasecmp(current_tenant->last_name, last_name) == 0) {
                //if tenant is found
                tenant_found = 1;
                break;
            }
            //prev to current
            previous_tenant = current_tenant;
            //current to next tenant
            current_tenant = current_tenant->next_tenants;
        }
        //if tenant is found breaks
        if (tenant_found) break;
        //current unit to next unit
        current_unit = current_unit->next_units;
    }

    //if tenant is not found prints error message
    if (tenant_found == 0) {
        printf("\nError: Tenant not found!\n");
        return;
    }

    //declare variable
    char confirm[4];

    //ask user for confirmation of removal
    printf("Are you sure you want to remove tenant %s %s? (yes/no): ", first_name, last_name);
    scanf("%3s", confirm);

    //regardless of cases if its not yes then deletion is canceled
    if (strcasecmp(confirm, "yes") != 0 && strcasecmp(confirm, "YES") != 0) {
        printf("\nDeletion canceled!\n");
        return;
    }

    //remove the tenant from the unit
    if (previous_tenant == NULL) {
        //tenant is the first tenant in the unit
        current_unit->uns_tenants = current_tenant->next_tenants;
    } 
    else {
        //tenant is not the first tenant
        previous_tenant->next_tenants = current_tenant->next_tenants;
    }

    //decrement current_occupants
    current_unit->current_occupants--;
    //prints successful message if removed
    printf("\nTenant %s %s has been removed!\n", first_name, last_name);
}

//function for editing a tenant information
void edit_tenant(struct units *uns, struct tenants *tens){
    //check if there are any units added, if not then prints error message
    if (tens == NULL){
        printf("There are no tenants as of the moment!\n");
        return;
    }

    //declare variables
    char first_name[50];
    char last_name[50];

    //ask user for first name
    printf("Enter first name: ");
    scanf(" %[^\n]", first_name);

    //ask user for last name
    printf("Enter last name: ");
    scanf(" %[^\n]", last_name);

    //initialize current unit pointer to the list of units
    struct units *current_unit = uns;
    //declare a pointer current tenant
    struct tenants *current_tenant;
    //declare and initialize variable if tenant is found
    int tenant_found = 0;

    //traverse thru list if current unit is not empty
    while (current_unit != NULL) {
        //initialize current tenant with the list of tenants
        current_tenant = current_unit->uns_tenants;
        //traverse thru list if current unit is not empty
        while (current_tenant != NULL) {
            //checks if first and last names are in the list regardless of its cases
            if (strcasecmp(current_tenant->first_name, first_name) == 0 &&
                strcasecmp(current_tenant->last_name, last_name) == 0) {
                //if tenant is found
                tenant_found = 1;
                break;
            }
            //current tenant to next tenant
            current_tenant = current_tenant->next_tenants;
        }
        //if found then it breaks
        if (tenant_found) break;
        //current unit to the next unit
        current_unit = current_unit->next_units;
    }

    //if not found then prints error message
    if (tenant_found == 0) {
        printf("\nError: Tenant not found!\n");
        return;
    }

    //declare variable
    char new_mobile_number[12];

    //ask for new mobile number of the tenant
    printf("Enter new mobile number (11 digit format ie. 09123456789): ");
    scanf("%s", new_mobile_number);

    //update the mobile number of the tenant
    strcpy(current_tenant->mobile_number, new_mobile_number);
    printf("\nMobile number updated successfully!\n");
}

//function for printing the masterlist of occupants
void print_masterlist_occupants(struct units *uns) {
    //check if there are tenants, print an error message if none
    if (uns == NULL) {
        printf("There are no tenants as of the moment!\n");
        return;
    }

    //initialize pointer to the list of units
    struct units *current_unit = uns;

    //print the sorted list of occupants
    printf("======= MASTER LIST OF OCCUPANTS =======\n");
    
    
    //empty linked list to track of printed tenants
    struct tenants *printed_tenants = NULL;

    //traverse each unit
    while (current_unit != NULL) {
        printf("Unit: %s\n", current_unit->unit_name);

        //declare and initialize variable for each tenants
        int tenant_number = 1;

        //check if there are any occupants in the unit
        if (current_unit->uns_tenants == NULL) {
            printf("Sorry, there are no occupants at the moment!\n\n");
        } 
        else {
            //print each tenant in the unit, ensuring no duplicates are printed
            struct tenants *current_tenant = current_unit->uns_tenants;
            while (current_tenant != NULL) {
                //check if the current tenant has already been printed
                //declare and initialize if the tenant is already printed
                int tenant_printed = 0;
                //assign pointer temp to the printed tenants
                struct tenants *temp = printed_tenants;
                //traverses thru the list if its not empty
                while (temp != NULL) {
                    //checks name regardless of its cases
                    if (strcasecmp(temp->first_name, current_tenant->first_name) == 0 &&
                        strcasecmp(temp->last_name, current_tenant->last_name) == 0) {
                        //if found
                        tenant_printed = 1;
                        return;
                    }
                    //updates temp to the next tenant
                    temp = temp->next_tenants;
                }

                //tf the tenant hasn't been printed yet, print their details and add them to printed_tenants
                if (tenant_printed == 0) {
                    printf("Tenant [%d]:\n", tenant_number++);
                    printf("Name: %s, %s\n", current_tenant->last_name, current_tenant->first_name);
                    printf("Mobile Number: %s\n\n", current_tenant->mobile_number);

                    //add the tenant to printed_tenants
                    //memory allocation for new printed tenants
                    struct tenants *new_printed_tenant = (struct tenants *)malloc(sizeof(struct tenants));
                    //copies the data
                    strcpy(new_printed_tenant->first_name, current_tenant->first_name);
                    strcpy(new_printed_tenant->last_name, current_tenant->last_name);
                    strcpy(new_printed_tenant->mobile_number, current_tenant->mobile_number);
                    //updates next tenants to the printed tenants
                    new_printed_tenant->next_tenants = printed_tenants;
                    //updates printed tenants to new printed one
                    printed_tenants = new_printed_tenant;
                }

                //move to the next tenant in the unit
                current_tenant = current_tenant->next_tenants;
            }
        }

        //move to the next unit
        current_unit = current_unit->next_units;
    }
}

//function for printing the occupants per unit
void print_occupants_unit(struct units *uns) {
    //check if there are any units added then prints error message
    if (uns == NULL) {
        printf("There are no units available!\n");
        return;
    }

    //declare variable
    char unit_name[50];

    //ask for the unit name
    printf("Enter the unit name: ");
    scanf(" %[^\n]", unit_name);
    
    //convert the input unit name to lowercase
    for (int i = 0; unit_name[i]; i++) {
        unit_name[i] = tolower(unit_name[i]);
    }
    
    //declare and initialize variable for each tenant number
    int tenant_number = 1;
    
    //initialize current unit pointer to the list of units
    struct units *current_unit = uns;
    //search for the unit with the given name (case insensitive this time)
    while (current_unit != NULL) {
        //convert the stored unit name to lowercase for comparison
        //declare variable to hold the unit name
        char stored_unit_name[50];
        //copies the unit name and loops down to convert to lowercase
        strcpy(stored_unit_name, current_unit->unit_name);
        for (int i = 0; stored_unit_name[i]; i++) {
            stored_unit_name[i] = tolower(stored_unit_name[i]);
        }

        //comparison of stored unit name to the unit name in the list
        if (strcasecmp(stored_unit_name, unit_name) == 0) {
            //print the list of occupants
            printf("\n=== Occupants of Unit %s ===\n", current_unit->unit_name);

            //check if there are any occupants in the unit if there's not then prints an error message
            if (current_unit->uns_tenants == NULL) {
                printf("Sorry, there are no occupants at the moment!\n\n");
                return;
            }

            
            //initialize prev tenant pointer to empty
            struct tenants *prev_tenant = NULL;
            //initialize current tenant pointer to the list of tenants
            struct tenants *current_tenant = current_unit->uns_tenants;

            //print each tenant in the unit, ensuring no duplicates are printed
            while (current_tenant != NULL) {
                //check if the current tenant has already been printed
                //declare and initialize printed tenants if found
                int tenant_printed = 0;
                //initialize temp pointer to the head tenant
                struct tenants *temp = current_unit->uns_tenants;
                //traverse temp is not the same as the current tenant
                while (temp != current_tenant) {
                    //comparison of the entered name and the stored name at the list
                    if (strcasecmp(temp->first_name, current_tenant->first_name) == 0 &&
                        strcasecmp(temp->last_name, current_tenant->last_name) == 0) {
                        //if found
                        tenant_printed = 1;
                        return;
                    }
                    //temp to next tenant
                    temp = temp->next_tenants;
                }

                //if the tenant hasn't been printed yet then print their details
                if (tenant_printed == 0) {
                    printf("Tenant [%d]:\n", tenant_number++);
                    printf("Name: %s, %s\n", current_tenant->last_name, current_tenant->first_name);
                    printf("Mobile Number: %s\n\n", current_tenant->mobile_number);
                }

                //move to the next tenant
                current_tenant = current_tenant->next_tenants;
            }

            return;
        }
        //move to the next unit
        current_unit = current_unit->next_units;
    }

    //if the unit with the given name is not found
    printf("\nError: Unit '%s' not found!\n", unit_name);
}

//function for removing a unit
void remove_unit(struct units **uns, struct tenants **tens) {
    //checks the list if its empty then prints an error message
    if (*uns == NULL) {
        printf("There are no units available!\n");
        return;
    }

    //prints units available to remove
    printf("===== UNITS ======\n");
    struct units *temp = *uns;
    while (temp != NULL) {
        printf("- %s (Occupants: %d/%d)\n", temp->unit_name, temp->current_occupants, temp->max_occupants);
        temp = temp->next_units;
    }
    printf("============================\n");

    //asks for unit name to remove
    char unit_name[50];
    printf("Enter the unit name to remove: ");
    scanf(" %[^\n]", unit_name);

    //initialize pointer units to the head list of units and to empty
    struct units *current_unit = *uns;
    struct units *previous_unit = NULL;

    //traverses while the current_unit is not empty
    while (current_unit != NULL) {
        //checks if the unit name entered and the unit name in the list are similar
        if (strcasecmp(current_unit->unit_name, unit_name) == 0) {
            //checks if the current unit has tenants
            if (current_unit->uns_tenants != NULL) {
                //initialize pointers to traverse through the list of tenants
                struct tenants *current_tenant = current_unit->uns_tenants;
                struct tenants *prev_tenant = NULL;

                //traverses while the list is not empty
                while (current_tenant != NULL) {
                    //asks user if they wanted to be move or remove
                    char move_or_remove[10];
                    printf("\nDo you want to move or remove tenant %s %s? (move/remove): ", current_tenant->first_name, current_tenant->last_name);
                    scanf("%9s", move_or_remove);

                    //if they chose move
                    if (strcasecmp(move_or_remove, "move") == 0) {
                        //prints the available units
                        printf("\n===== AVAILABLE UNITS ======\n");
                        temp = *uns;
                        while (temp != NULL) {
                            if (temp != current_unit && temp->current_occupants < temp->max_occupants) {
                                printf("- %s (Occupants: %d/%d)\n", temp->unit_name, temp->current_occupants, temp->max_occupants);
                            }
                            temp = temp->next_units;
                        }
                        printf("===========================\n");

                        //asks user for the new unit they want to move in
                        char new_unit_name[50];
                        printf("\nEnter the name of the unit to move tenant %s %s to: ", current_tenant->first_name, current_tenant->last_name);
                        scanf(" %[^\n]", new_unit_name);

                        //initialize pointer to the list of units
                        struct units *new_unit = *uns;
                        //traverses the list of units if the entered new unit is in the list
                        while (new_unit != NULL && strcasecmp(new_unit->unit_name, new_unit_name) != 0) {
                            new_unit = new_unit->next_units;
                        }

                        //check if the new unit exists and if it has available space for new occupants
                        if (new_unit != NULL && new_unit->current_occupants < new_unit->max_occupants) {
                            //prepare to move a tenant from the current unit to the new unit
                            struct tenants *tenant_to_move = current_tenant;
                            //if the current tenant is the head of the linked list, update the head pointer
                            if (prev_tenant == NULL) {
                                current_unit->uns_tenants = current_tenant->next_tenants;
                            } 
                            //otherwise, adjust the pointers to skip the current tenant
                            else {
                                prev_tenant->next_tenants = current_tenant->next_tenants;
                            }
                            //move to the next tenant in the current unit
                            current_tenant = current_tenant->next_tenants;

                            //update pointers to add the moved tenant to the new unit's tenant list
                            tenant_to_move->next_tenants = new_unit->uns_tenants;
                            new_unit->uns_tenants = tenant_to_move;
                            //adjust occupant counts for both units
                            current_unit->current_occupants--;
                            new_unit->current_occupants++;
                            printf("\nTenant %s %s has been moved to unit %s!\n", tenant_to_move->first_name, tenant_to_move->last_name, new_unit->unit_name);
                        } 
                        //if the unit is full then prints error message
                        else {
                            printf("\nError: Unit '%s' is not available or is full.\n", new_unit_name);
                            prev_tenant = current_tenant;
                            current_tenant = current_tenant->next_tenants;
                        }
                        continue;
                    } 
                    //if they chose remove
                    else if (strcasecmp(move_or_remove, "remove") == 0) {
                        //initialize pointer to the current tenant list
                        struct tenants *temp_tenant = current_tenant;
                        //if the previous tenant pointer is NULL, the current tenant is the head of the list
                        if (prev_tenant == NULL) {
                            current_unit->uns_tenants = current_tenant->next_tenants;
                        } 
                        //update the previous tenant's next pointer to skip the current tenant
                        else {
                            prev_tenant->next_tenants = current_tenant->next_tenants;
                        }
                        //move to the next tenant in the list
                        current_tenant = current_tenant->next_tenants;
                        printf("\nTenant %s %s has been removed from the unit!\n", temp_tenant->first_name, temp_tenant->last_name);
                        //free the memory allocated for the removed tenant
                        free(temp_tenant);
                        //continues the next iteration of the loop
                        continue;
                    } 
                    //if invalid choice then prints error message
                    else {
                        printf("Error: Invalid choice. Please enter a valid choice!\n");
                    }

                    //updates the list of tenant
                    prev_tenant = current_tenant;
                    current_tenant = current_tenant->next_tenants;
                }
            }
            //check if the current unit is the first one in the list
            if (previous_unit == NULL) {
                //update the head pointer to skip the current unit
                *uns = current_unit->next_units;
            } 
            else {
                //update the previous unit's next pointer to skip the current unit
                previous_unit->next_units = current_unit->next_units;
            }

            //frees the allocated memory
            free(current_unit);
            printf("\nUnit '%s' removed successfully!\n", unit_name);
            return;
        }
        previous_unit = current_unit;
        current_unit = current_unit->next_units;
    }

    //error message if unit is not found
    printf("Error: Unit '%s' not found!\n", unit_name);
}

//function for automatically saving
void save_tenants(struct units *uns) {
    //opens the file with name tenants.txt to write
    FILE *file = fopen("tenants.txt", "w");
    //if it's empty then prints this
    if (file == NULL) {
        printf("File has no content! Starting with an empty file.\n");
        return;
    }

    //initialize current unit pointer to the list of units
    struct units *current_unit = uns;
    //traverse through the list if it's not empty
    while (current_unit != NULL) {
        //saves the data for units info
        fprintf(file, "%s\n%d\n%d\n", current_unit->unit_name, current_unit->max_occupants, current_unit->current_occupants);
        //initialize current tenant with the list of tenants
        struct tenants *current_tenant = current_unit->uns_tenants;
        //traverse through the list of tenants if it's not empty
        while (current_tenant != NULL) {
            //saves the data for tenants info
            fprintf(file, "%s\n%s\n%s\n", current_tenant->first_name, current_tenant->last_name, current_tenant->mobile_number);
            current_tenant = current_tenant->next_tenants;
        }
        //update current unit to the next unit
        current_unit = current_unit->next_units;
    }

    //closes file to avoid mishandling
    fclose(file);
    //print to let the user know that data saves successfully
    printf("Data saved successfully.\n");
}

//function for automatically loading
void load_tenants(struct units **uns) {
    //open file to read
    FILE *file = fopen("tenants.txt", "r");
    //if the file is empty then print this
    if (file == NULL) {
        printf("\nFile has no content! Starting with an empty file.\n");
        return;
    }

    //initialize to empty for the new and last units
    struct units *new_units = NULL;
    struct units *last_unit = NULL;
    //declare variables
    char unit_name[50];
    int max_occupants;
    int current_occupants;

    //read a line from the file into unit_name until encountering a newline character
    while (fscanf(file, " %[^\n]", unit_name) != EOF) {
        //read integers from the file and store them in max_occupants and current_occupants
        fscanf(file, "%d\n%d\n", &max_occupants, &current_occupants);
        //memory allocation for new unit
        struct units *new_unit = (struct units *)malloc(sizeof(struct units));

        //copy unit name into new_unit->unit_name
        strcpy(new_unit->unit_name, unit_name);
        //set the max occupants and current occupants fields of new_unit
        new_unit->max_occupants = max_occupants;
        new_unit->current_occupants = current_occupants;
        //initialize to empty
        new_unit->uns_tenants = NULL;
        new_unit->next_units = NULL;

        //if new_units is empty then update the new_units to the new unit
        if (new_units == NULL) {
            new_units = new_unit;
        } 
        //if not then next_units is set to new unit
        else {
            last_unit->next_units = new_unit;
        }
        //update last_unit to new_unit
        last_unit = new_unit;

        //iterate over the number of current occupants
        for (int i = 0; i < current_occupants; i++) {
            //memory allocation for new tenant
            struct tenants *new_tenant = (struct tenants *)malloc(sizeof(struct tenants));
            //save data for tenant information
            fscanf(file, " %[^\n]", new_tenant->first_name);
            fscanf(file, " %[^\n]", new_tenant->last_name);
            fscanf(file, " %[^\n]", new_tenant->mobile_number);
            //initialize next_tenants to null
            new_tenant->next_tenants = NULL;

            //insert the new tenant in a sorted order
            //initialize head to the list of tenants
            struct tenants **head = &(new_unit->uns_tenants);
            //check if the list is empty or if the new tenant should be first
            if (*head == NULL || strcmp((*head)->last_name, new_tenant->last_name) > 0 ||
                (strcmp((*head)->last_name, new_tenant->last_name) == 0 && strcmp((*head)->first_name, new_tenant->first_name) > 0)) {
                //insert new tenant at the head
                new_tenant->next_tenants = *head;
                //update head to the new tenant
                *head = new_tenant;
            } 
            else {
                //initialize current to the head of the list
                struct tenants *current = *head;
                //traverse the list to find the correct position
                while (current->next_tenants != NULL &&
                      (strcmp(current->next_tenants->last_name, new_tenant->last_name) < 0 || 
                      (strcmp(current->next_tenants->last_name, new_tenant->last_name) == 0 && strcmp(current->next_tenants->first_name, new_tenant->first_name) < 0))) {
                    //move to the next tenant
                    current = current->next_tenants;
                }
                //insert new tenant in the correct position
                new_tenant->next_tenants = current->next_tenants;
                //update the next tenant pointer
                current->next_tenants = new_tenant;
            }
        }
    }

    //close file to avoid mishandling
    fclose(file);
    //update the list of units to new units
    *uns = new_units;
    //print to let the user know that data is loaded successfully from the last execution
    printf("\nData loaded successfully!\n");
}

int main(){
    //initialize uns to null
    struct units *uns = NULL;
    struct tenants *tens = NULL;
    
    //declare variable choice
    int choice;
        
    //loads automatically when executed
    load_tenants(&uns);

    //menu display
    do {
        printf("\n======== MENU ========\n");
        printf("[1] Add Unit\n");
        printf("[2] Add Tenant\n");
        printf("[3] Search Tenant\n");
        printf("[4] Move Tenant\n");
        printf("[5] Remove Tenant\n");
        printf("[6] Edit Tenant\n");
        printf("[7] Print Master List of Occupants\n");
        printf("[8] Print List of Occupants Per Unit\n");
        printf("[9] Remove Unit\n");
        printf("[10] Exit\n");
        printf("=======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice) {
            case 1:
                //call for adding unit
                add_unit(&uns);
                break;
            case 2:
                //call for adding tenant
                add_tenant(uns, &tens);
                break;
            case 3:
                //call for searching a tenant
                search_tenant(uns, tens);
                break;
            case 4:
                //call for moving a tenant
                move_tenant(uns, tens);
                break;
            case 5:
                //call for removing a tenant
                remove_tenant(&uns, &tens);
                break;
            case 6:
                //call for editing a tenant
               edit_tenant(uns, tens);
                break;
            case 7:
                //call for printing the masterlist of occupants
                print_masterlist_occupants(uns);
                break;
            case 8:
                //call for printing the occupants per unit
                print_occupants_unit(uns);
                break;
            case 9:
                //call for removing a unit
                remove_unit(&uns, &tens);
                break;
            case 10:
                //automatically saves data when exiting
                save_tenants(uns);
                //prints exiting message
                printf("Goodbye!\n");
                break;
            default:
                //if invalid
                printf("\nError: Invalid choice!\n");
        }
    } while(choice != 10); //if choice is not 10

    return 0;
}