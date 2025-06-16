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
//大小写转换 
void toLowerCase(char* str)
{
	for(int i=0;str[i];i++)
	{
		str[i]=tolower(str[i]);
	}
}
/*
对每个读取的单词:
1.复制到currentWord
2.转换为小写
3.与目标单词比较(strcmp)
4.如果匹配:
复制定义到输出参数definition
设置found=1
跳出循环
*/
//查询单词 
int searchWord(const char*word, char* definition)//word:查找的单词；definition:存找到的定义(会输出) 
{
    FILE *file = fopen("dictionary.txt", "rb");
    if (file == NULL) 
	return 0;

    Dentry entry;//存从文件读取到的单词词条 
    int found = 0;//是否找到 
    char lowerWord[1024];
    strcpy(lowerWord, word);
    toLowerCase(lowerWord);
    
    //遍历文件查找单词 
    while (fread(&entry, sizeof(Dentry), 1, file))//从文件读取一个Dentry结构体 
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

//加单词 
int addWord(const char*word, const char*definition)//要添加的单词和单词的定义 
{
    FILE *file = fopen("dictionary.txt", "a");//打开词典文件并追加写入(二进制模式避免字符转换问题) 
    if (file == NULL) return 0;

    Dentry entry={0};
    strncpy(entry.word, word, MaxWordlen-1);
    strncpy(entry.def, definition, MaxDeflen-1);
    entry.word[MaxWordlen-1] = '\0';
    entry.def[MaxDeflen-1] = '\0';//确保字符串以'\0'结束 
    
    fwrite(&entry, sizeof(Dentry), 1, file);//将新单词写入文件 
    fclose(file);
    return 1;
}

//列出词典中的所有单词
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
//删除单词 
int deleteWord(const char *word) 
{
    FILE *file = fopen("dictionary.txt", "rb");
    if (file == NULL) return 0;
	//临时文件 
    FILE *tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL) 
	{
        fclose(file);
        return 0;
    }
	//处理大小写 
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
			//不是要删除的单词则写入临时文件 
            fwrite(&entry, sizeof(Dentry), 1, tempFile);
        } else 
		{
            deleted = 1;
        }
    }
    //关闭文件并替换 
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
		printf("*---------在线词典-----------*\n");
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
            case 1: // 搜索单词
                printf("Enter word to search: ");
                fgets(word, MaxWordlen, stdin);
                word[strcspn(word, "\n")] = '\0';
                
                if (searchWord(word, definition)) {
                    printf("\nDefinition: %s\n", definition);
                } else {
                    printf("\nWord '%s' not found\n", word);
                }
                break;
                
            case 2: // 添加单词
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
                
            case 3: // 删除单词
                printf("Enter word to delete: ");
                fgets(word, MaxWordlen, stdin);
                word[strcspn(word, "\n")] = '\0';
                
                if (deleteWord(word)) {
                    printf("\nWord '%s' deleted successfully\n", word);
                } else {
                    printf("\nWord '%s' not found\n", word);
                }
                break;
                
            case 4: // 列出所有单词
                listAllWords();
                break;
                
            case 5: // 退出
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
