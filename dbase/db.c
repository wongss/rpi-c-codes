// db.c

#include <sqlite3.h>
#include <string.h>
#include "db.h"

char tableName[65]; // table name cannot exceed 64 characters
char queryStr[1024]; // shared query string

static int notFirstCall = 0, rowCount = 0;
static void msqliteExecInit( void ) {
	notFirstCall = 0;
	rowCount = 0;
}
// -------------------------------------------------
// call back function passed to sqlite_exe()
// called once for each row obtained by sqlite_exec()
// For multiple rows, it would be called multiple times
// return value:
// 0 => sqlite_exec() would return SQLITE_OK at the last row
// non-zero => sqlite_exec() stop returns SQLITE_ABORT
static int showRowsCallBack(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	if (notFirstCall == 0) {
		printf("\n");
		printf("%12s","row");
		for(i=0; i<argc; i++) {	// how many columns
			printf("%12s", azColName[i]); // column name
		}
		notFirstCall = 1; // no first call anymore		
		rowCount = 0;
	}
	rowCount++;
	printf("\n%12d",rowCount);
	for(i=0; i<argc; i++) {	// how many columns
		printf("%12s", argv[i] ? argv[i] : "NULL"); // column value in string form
	}
	fflush(stdout);
	return 0;
}
// -------------------------------------------------
static int tableNameCallBack(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	if (notFirstCall == 0) {
		printf("\n%12s","table");
		for(i=0; i<argc; i++) {	// how many columns
			printf("%12s", azColName[i]); // column name
		}
		notFirstCall = 1; // no first call anymore		
		rowCount = 0;
	}
	rowCount++;
	printf("\n%12d",rowCount);
	for(i=0; i<argc; i++) {	// how many columns
		printf("%12s", argv[i] ? argv[i] : "NULL"); // column value in string form
		if (rowCount==1 && i == 0) {
			// first table
			strcpy(tableName,argv[i]);
		}
	}
	fflush(stdout);
	return 0;
}
// -------------------------------------------------
static int colInfoCallBack(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	if (notFirstCall == 0) {
		printf("\n");
		for(i=0; i<3; i++) {	// 3 columns will do, others not useful
			printf("%12s", azColName[i]); // column name
		}
		notFirstCall = 1; // no first call anymore	
		rowCount = 0;	
	}
	rowCount++;
	printf("\n");
	for(i=0; i<3; i++) {	// 3 cols will do for columns info
		printf("%12s", argv[i] ? argv[i] : "NULL");
	}
	fflush(stdout);
	return 0;
}
// -------------------------------------------------
void sysInit ( void ) {
	printf("Testing database"); fflush(stdout);
}
// -------------------------------------------------
void sysClose( int status ) {
	printf("\nCleaning up & exit with status %d\n",status);
	exit(status);
}
// -------------------------------------------------
// returns 1 for yes, 0 for no
int msqliteFileExist( const char *fname) {
	sqlite3 *db;
	int rc;
	// cannot use sqlite3_open() as it would create the file if it does not exist
	rc = sqlite3_open_v2(fname,&db,SQLITE_OPEN_READONLY,NULL);
	sqlite3_close(db);
	if( rc!=SQLITE_OK ){
		return 0;
	}
	return 1;
}
// -------------------------------------------------
int msqliteExec(sqlite3 *db, const char *query,
		int (*callBack)(void *NotUsed, int argc, char **argv, char **azColName)
	) {
	int rc;
	char *zErrMsg;
	msqliteExecInit();
	rc = sqlite3_exec(db, query, callBack, 0, &zErrMsg); // query and process each row
	if( rc!=SQLITE_OK ) {
		fprintf(stderr, "\nSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	return rowCount; // callBack function is to update this !
}
// -------------------------------------------------
int showTableNames(sqlite3 *db) {
	int rc;
	printf("\nDisplay Table(s) name(s)"); fflush(stdout);
	if ((rc=msqliteExec(db, "select name from sqlite_master where type in ('table');", tableNameCallBack)) == 0) {
		printf("\nNo table found");
	}
	printf("\n");
	return rc;
}
// -------------------------------------------------
int displayAllData(sqlite3 *db, const char *tname) {
	int rc;
	printf("\nDisplay All data in the table '%s'",tname); fflush(stdout);
	sprintf(queryStr,"SELECT * FROM %s;",tname);
	if ((rc=msqliteExec(db, queryStr, showRowsCallBack)) == 0) {
		printf("\nNo data found");
	}
	printf("\n");
	return rc;
}
// -------------------------------------------------
int showTableInfo(sqlite3 *db, const char *tname) {
	int rc;
	printf("\nDisplay Table Columns info in table '%s'",tname); fflush(stdout);
	sprintf(queryStr,"PRAGMA table_info(%s);",tname);
	if ((rc=msqliteExec(db, queryStr, colInfoCallBack)) == 0) {
		printf("\nNo column found");
	}
	printf("\n");
	return rc;
}
// -------------------------------------------------
/*
int insertAData(sqlite3 *db, const char *tname) {
	int rc;
	printf("\nInsert a data into the table '%s'",tname); fflush(stdout);
	sprintf(queryStr,"INSERT INTO user VALUES(876,\"Ken\",112,223);");
	if ((rc=msqliteExec(db, queryStr, showRowsCallBack)) == 0) {
		printf("\nNo data returned during insert");
	}
	printf("\n");
	return rc;
}
*/
// -------------------------------------------------
void readNExec( sqlite3 *db ) {
	int rc;
	while(1) {
		printf("\nEnter SQL statement ending with ';' (\"exit\" to quit):");
		gets(queryStr);
		if (strncmp(queryStr,"exit",5) == 0) {
			return;
		}
		if ((rc=msqliteExec(db, queryStr, showRowsCallBack)) == 0) {
			printf("\nNo data returned by the statement");
		}
		// any error during the execution would be reported by msqliteExec()
		printf("\n");
	}
}
// -------------------------------------------------
int main ( int argc, char **argv ) {
	sqlite3 *db;
	int rc;

	if (argc != 2) {
		printf("\nUsage: %s dbname\n",argv[0]);
		exit(1);
	}
	sysInit();
	if (!msqliteFileExist(argv[1])) {
		printf("\nSqlite file %s does not exist\n",argv[1]);
		exit(1);
	}
	rc = sqlite3_open(argv[1],&db);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "\nCan't open database. Err msg: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	if ((rc = showTableNames(db)) < 0) {
		printf("\nShow table name error, quit"); fflush(stdout);
		goto quit;
	}
	if (rc == 0) {
		printf("\nNo table found thus quit"); fflush(stdout);
		goto quit;
	}
	printf("\nFirst table name is: %s\n",tableName);
	if (showTableInfo(db, tableName) < 0) {
		printf("\nShow table info error, quit"); fflush(stdout);
		goto quit;
	}
	if (displayAllData(db,tableName) < 0)  {
		printf("\nDisplay all data error, quit"); fflush(stdout);
		goto quit;
	}
/*	if (insertAData(db,tableName) < 0)  {
		printf("\nDisplay all data error"); fflush(stdout);
		goto quit;
	}
*/	readNExec(db);
	printf("\nEnd of processing\n");
quit:
	sqlite3_close(db); // do inside sysClose()?
	sysClose(0);
	return 0;
}
