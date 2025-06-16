#define MaxWordlen 32
#define MaxDeflen 32
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
typedef struct 
{
	char word[MaxWordlen];
	char def[MaxDeflen];
}Dentry;
//��Сдת�� 
void toLowerCase(char* str)
{
	for(int i=0;str[i];i++)
	{
		str[i]=tolower(str[i]);
	}
}
/*
��ÿ����ȡ�ĵ���:
1.���Ƶ�currentWord
2.ת��ΪСд
3.��Ŀ�굥�ʱȽ�(strcmp)
4.���ƥ��:
���ƶ��嵽�������definition
����found=1
����ѭ��
*/
//��ѯ���� 
int searchWord(const char*word, char* definition)//word:���ҵĵ��ʣ�definition:���ҵ��Ķ���(�����) 
{
    FILE *file = fopen("dictionary.txt", "rb");
    if (file == NULL) 
	return 0;

    Dentry entry;//����ļ���ȡ���ĵ��ʴ��� 
    int found = 0;//�Ƿ��ҵ� 
    char lowerWord[1024];
    strcpy(lowerWord, word);
    toLowerCase(lowerWord);
    
    //�����ļ����ҵ��� 
    while (fread(&entry, sizeof(Dentry), 1, file))//���ļ���ȡһ��Dentry�ṹ�� 
	{
        char currentWord[1024];
        strcpy(currentWord, entry.word);
        toLowerCase(currentWord);
        
        if (strcmp(currentWord, lowerWord) == 0) {
            strcpy(definition, entry.def);
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

//�ӵ��� 
int addWord(const char*word, const char*definition)//Ҫ��ӵĵ��ʺ͵��ʵĶ��� 
{
    FILE *file = fopen("dictionary.txt", "a");//�򿪴ʵ��ļ���׷��д��(������ģʽ�����ַ�ת������) 
    if (file == NULL) return 0;

    Dentry entry={0};
    strncpy(entry.word, word, MaxWordlen-1);
    strncpy(entry.def, definition, MaxDeflen-1);
    entry.word[MaxWordlen-1] = '\0';
    entry.def[MaxDeflen-1] = '\0';//ȷ���ַ�����'\0'���� 
    
    fwrite(&entry, sizeof(Dentry), 1, file);//���µ���д���ļ� 
    fclose(file);
    return 1;
}

//�г��ʵ��е����е���
void listAllWords() 
{
    FILE *file = fopen("dictionary.txt", "rb");
    if (file == NULL) {
        printf("Dictionary is empty\n");
        return ;
    }

    Dentry entry;
    int count = 0;
    
    printf("\nAll words in dictionary:\n");
    while (fread(&entry, sizeof(Dentry), 1, file)) 
    {
        printf("%d. %s: %s\n", ++count, entry.word, entry.def);
    }
    
    if (count == 0) 
    {
        printf("Dictionary is empty\n");
    }
    
    fclose(file);
}
//ɾ������ 
int deleteWord(const char *word) 
{
    FILE *file = fopen("dictionary.txt", "rb");
    if (file == NULL) return 0;
	//��ʱ�ļ� 
    FILE *tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL) 
	{
        fclose(file);
        return 0;
    }
	//�����Сд 
    char lowerWord[MaxWordlen];
    strcpy(lowerWord, word);
    toLowerCase(lowerWord);
    
    Dentry entry;
    int deleted = 0;
    while (fread(&entry, sizeof(Dentry), 1, file)) 
	{
        char currentWord[MaxWordlen];
        strcpy(currentWord, entry.word);
        toLowerCase(currentWord);
        
        if (strcmp(currentWord, lowerWord) != 0) 
		{
			//����Ҫɾ���ĵ�����д����ʱ�ļ� 
            fwrite(&entry, sizeof(Dentry), 1, tempFile);
        } else 
		{
            deleted = 1;
        }
    }
    //�ر��ļ����滻 
    fclose(file);
    fclose(tempFile);
    remove("dictionary.txt");
    rename("temp.txt", "dictionary.txt");
    
    return deleted;
}
int main()
{
	int choice;
	char word[MaxWordlen],definition[MaxDeflen];
	
	while(1)
	{
		printf("******************************\n");
		printf("*---------���ߴʵ�-----------*\n");
		printf("******************************\n");
		printf("*       1. Search Word       *\n");
		printf("*       2. Add Word          *\n");
		printf("*       3. Delete Word       *\n");
		printf("*       4. List All Words    *\n");
		printf("*       5. Exit              *\n");
		printf("******************************\n");
		printf("Enter your choice: \n");
		scanf("%d",&choice);
		getchar();
		
		switch (choice) {
            case 1: // ��������
                printf("Enter word to search: ");
                fgets(word, MaxWordlen, stdin);
                word[strcspn(word, "\n")] = '\0';
                
                if (searchWord(word, definition)) {
                    printf("\nDefinition: %s\n", definition);
                } else {
                    printf("\nWord '%s' not found\n", word);
                }
                break;
                
            case 2: // ��ӵ���
                printf("Enter word: ");
                fgets(word, MaxWordlen, stdin);
                word[strcspn(word, "\n")] = '\0';
                printf("Enter definition: ");
                fgets(definition, MaxDeflen, stdin);
                definition[strcspn(definition, "\n")] = '\0';
                
                if (addWord(word, definition)) {
                    printf("\nWord '%s' added successfully\n", word);
                } else {
                    printf("\nFailed to add word\n");
                }
                break;
                
            case 3: // ɾ������
                printf("Enter word to delete: ");
                fgets(word, MaxWordlen, stdin);
                word[strcspn(word, "\n")] = '\0';
                
                if (deleteWord(word)) {
                    printf("\nWord '%s' deleted successfully\n", word);
                } else {
                    printf("\nWord '%s' not found\n", word);
                }
                break;
                
            case 4: // �г����е���
                listAllWords();
                break;
                
            case 5: // �˳�
                printf("\nGoodbye!\n");
                exit(0);
                
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
		
		printf("\nPress Enter to continue...");
        getchar(); 
	}
	return 0;
}
