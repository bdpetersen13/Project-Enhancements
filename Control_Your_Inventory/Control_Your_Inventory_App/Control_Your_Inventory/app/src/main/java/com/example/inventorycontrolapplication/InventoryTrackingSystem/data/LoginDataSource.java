package com.example.inventorycontrolapplication.data;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.provider.BaseColumns;

import androidx.annotation.Nullable;

import com.example.inventorycontrolapplication.data.helpers.SqlDbHelper;
import com.example.inventorycontrolapplication.data.model.LoggedInUser;
import com.example.inventorycontrolapplication.data.model.SqlDbContract;

import java.io.IOException;
import java.util.Date;
import java.util.HashMap;

/**
 * Class that handles authentication w/ login credentials and retrieves user information.
 */
public class LoginDataSource {

    private SqlDbHelper DbHelper;

    public LoginDataSource(Context context) {
        DbHelper = new SqlDbHelper(context);
    }

    public Result<LoggedInUser> login(String username, String password) {

        try {
            // Projection
            String[] projection = {
                    BaseColumns._ID,
                    SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME,
                    SqlDbContract.AuthenticationEntry.COLUMN_NAME_PASSWORD,
                    SqlDbContract.AuthenticationEntry.COLUMN_NAME_LAST_LOGIN
            };
            // Filter results WHERE
            String selection = SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME + " = ?";
            String[] selectionArgs = { username };
            // Query Db
            Cursor cursor = queryLoginDatabase(projection, selection, selectionArgs, null);
            // Get first row & null check
            HashMap row = DbHelper.GetFirst(cursor);
            if (row.get(SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME).toString().isEmpty()
                    || row.get(SqlDbContract.AuthenticationEntry.COLUMN_NAME_PASSWORD).toString().isEmpty()) {
                return new Result.Error(new IOException("Error logging in"));
            }

            // Check if equal
            if (row.get(SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME).toString().equalsIgnoreCase(username)
                    && row.get(SqlDbContract.AuthenticationEntry.COLUMN_NAME_PASSWORD).toString().equalsIgnoreCase(password)) {
                return new Result.Success<>(new LoggedInUser
                        (
                                row.get(SqlDbContract.AuthenticationEntry._ID).toString(),
                                row.get(SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME).toString(),
                                row.get(SqlDbContract.AuthenticationEntry.COLUMN_NAME_LAST_LOGIN).toString()
                        )
                );
            } else {
                return new Result.Error(new IOException("Error logging in"));
            }
        } catch (Exception e) {
            return new Result.Error(new IOException("Error logging in", e));
        }
    }

    /*
        Function used to register the user in the database
     */
    public Result<LoggedInUser> register(String username, String password) {
        try {
            // Projection
            String[] projection = {
                    BaseColumns._ID,
                    SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME,
                    SqlDbContract.AuthenticationEntry.COLUMN_NAME_PASSWORD,
                    SqlDbContract.AuthenticationEntry.COLUMN_NAME_LAST_LOGIN
            };
            // Filter results WHERE
            String selection = SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME + " = ?";
            String[] selectionArgs = { username };
            // Query Db
            Cursor cursor = queryLoginDatabase(projection, selection, selectionArgs, null);
            // Get first row & null check
            HashMap row = DbHelper.GetFirst(cursor);
            if (row.isEmpty()) {
                // Insert new rows
                long id = insertLoginDatabase("", username, password);
                // Return logged in user
                return new Result.Success<>(new LoggedInUser
                        (
                                String.valueOf(id),
                                username,
                                ""
                        )
                );
            } else {
                return new Result.Error(new IOException("Account already registered"));
            }
        } catch (Exception e) {
            return new Result.Error(new IOException("Error registering account", e));
        }
    }

    /*
        Function to log out the user
     */
    public void logout() {
        // TODO: revoke authentication
    }

    /*
        Private Helpers
     */
    private Cursor queryLoginDatabase(String[] projection, @Nullable String selection,
                                 @Nullable String[] selectionArgs, @Nullable String sortOrder) {
        // Get DB
        SQLiteDatabase db = DbHelper.getReadableDatabase();
        // Query and return cursor
        return db.query(
                SqlDbContract.AuthenticationEntry.TABLE_NAME,          // The table to query
                projection,         // The array of columns to return (pass null to get all)
                selection,          // The columns for the WHERE clause
                selectionArgs,      // The values for the WHERE clause
                null,      // don't group the rows
                null,       // don't filter by row groups
                sortOrder           // The sort order
        );
    }

    private long insertLoginDatabase(String displayName, String username, String password) {
        // Get DB
        SQLiteDatabase db = DbHelper.getWritableDatabase();
        // Create content values
        ContentValues values = new ContentValues();
        values.put(SqlDbContract.AuthenticationEntry.COLUMN_NAME_NAME, displayName);
        values.put(SqlDbContract.AuthenticationEntry.COLUMN_NAME_USERNAME, username);
        values.put(SqlDbContract.AuthenticationEntry.COLUMN_NAME_PASSWORD, password);
        values.put(SqlDbContract.AuthenticationEntry.COLUMN_NAME_LAST_LOGIN, java.text.DateFormat.getDateTimeInstance().format(new Date()));
        // Insert the new row, returning the primary key value of the new row
        return db.insert(SqlDbContract.AuthenticationEntry.TABLE_NAME, null, values);
    }
}