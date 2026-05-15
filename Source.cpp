#include <stdio.h>
#include <stdlib.h>

//step 1 - Define structures
typedef struct Book {
	int isbn;
	float price;
} Book;

typedef struct TreeNode {
	Book info;
	struct TreeNode* left;
	struct TreeNode* right;
	int balanceFactor;
} TreeNode;

//step 2 - Book functions
Book read_book() {
	Book b;
	printf("Enter ISBN: ");
	scanf_s("%d", &b.isbn);

	printf("Enter price: ");
	scanf_s("%f", &b.price);

	return b;
}

void print_book(Book b) {
	printf("== Book info: ==\n");
	printf("- ISBN: %d\n", b.isbn);
	printf("- Price: $%.2f\n", b.price);
}

//step 3 - Create node
TreeNode* create_node(Book b) {
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));

	if (newNode != NULL) {
		newNode->info = b;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->balanceFactor = 0;
	}

	return newNode;
}

//step 4 - Tree traversal
void print(TreeNode* root) {
	if (root == NULL) {
		return;
	}
	print_book(root->info);
	print(root->left);
	print(root->right);
}

//step 5 - Tree height and Balance factor
int max(int a, int b) {
	return (a > b) ? a : b;
}

int tree_height(TreeNode* root) {
	if (root == NULL) {
		return 0;
	}
	int leftHeight = tree_height(root->left);
	int rightHeight = tree_height(root->right);

	return 1 + max(leftHeight, rightHeight);
}

int balance_factor(TreeNode* root) {
	if (root == NULL) {
		return 0;
	}
	return tree_height(root->left) - tree_height(root->right);
}

//step 6 - Rotations
TreeNode* rotate_right(TreeNode* y) {
	TreeNode* x = y->left;
	TreeNode* T2 = x->right;

	x->right = y;
	y->left = T2;

	y->balanceFactor = balance_factor(y);
	x->balanceFactor = balance_factor(x);

	return x;
}

TreeNode* rotate_left(TreeNode* x) {
	TreeNode* y = x->right;
	TreeNode* T2 = y->left;

	y->left = x;
	x->right = T2;

	x->balanceFactor = balance_factor(x);
	y->balanceFactor = balance_factor(y);

	return y;
}

TreeNode* rotate_left_right(TreeNode* root) {
	root->left = rotate_left(root->left);
	return rotate_right(root);
}

TreeNode* rotate_right_left(TreeNode* root) {
	root->right = rotate_right(root->right);
	return rotate_left(root);
}

//step 7 - Balancing function
TreeNode* balance(TreeNode* root) {
	if (root == NULL) {
		return NULL;
	}
	root->balanceFactor = balance_factor(root);
	if (root->balanceFactor == 2) {
		if (balance_factor(root->left) < 0) {
			return rotate_left_right(root);
		}
		else {
			return rotate_right(root);
		}
	}
	else if (root->balanceFactor == -2) {
		if (balance_factor(root->right) > 0) {
			return rotate_right_left(root);
		}
		else {
			return rotate_left(root);
		}
	}
	return root;
}

//step 8 - Insert node
TreeNode* insert_node(Book b, TreeNode* root) {
	if (root == NULL) {
		return create_node(b);
	}
	if (b.isbn < root->info.isbn) {
		root->left = insert_node(b, root->left);
	}
	else if (b.isbn > root->info.isbn) {
		root->right = insert_node(b, root->right);
	}
	else {
		return root;
	}
	return balance(root);
}

//step 9 - Delete node
TreeNode* min_value_node(TreeNode* node) {
	TreeNode* current = node;
	while (current != NULL && current->left != NULL) {
		current = current->left;
	}
	return current;
}

TreeNode* delete_node(int isbn, TreeNode* root);

TreeNode* delete_root_node(TreeNode* root) {
	if (root->left == NULL) {
		TreeNode* temp = root->right;
		free(root);
		return temp;
	}
	else if (root->right == NULL) {
		TreeNode* temp = root->left;
		free(root);
		return temp;
	}
	TreeNode* temp = min_value_node(root->right);
	root->info = temp->info;
	root->right = delete_node(temp->info.isbn, root->right);

	return root;
}

TreeNode* delete_node(int isbn, TreeNode* root) {
	if (root == NULL) {
		return NULL;
	}
	if (isbn < root->info.isbn) {
		root->left = delete_node(isbn, root->left);
	}
	else if (isbn > root->info.isbn) {
		root->right = delete_node(isbn, root->right);
	}
	else {
		root = delete_root_node(root);
	}
	if (root == NULL) {
		return NULL;
	}
	return balance(root);
}

//step 10 - Main function
void main() {
	TreeNode* root = NULL;
	int num_books, i, isbn_to_delete;

	printf("How many books do you want to insert? ");
	scanf_s("%d", &num_books);

	for (i = 0; i < num_books; i++) {
		printf("\nBook %d\n", i + 1);
		Book new_book = read_book();
		root = insert_node(new_book, root);
	}

	printf("\nAVL Tree Traversal (Preorder)\n");
	if (root == NULL) {
		printf("Tree is empty.\n");
	}
	else {
		print(root);
	}

	printf("\nEnter the ISBN of the book you want to delete: ");
	scanf_s("%d", &isbn_to_delete);

	root = delete_node(isbn_to_delete, root);
	printf("\nNode with ISBN %d deleted (if it existed).\n", isbn_to_delete);

	printf("\nAVL Tree Traversal After Deletion (Preorder)\n");
	if (root == NULL) {
		printf("Tree is empty.\n");
	}
	else {
		print(root);
	}
}