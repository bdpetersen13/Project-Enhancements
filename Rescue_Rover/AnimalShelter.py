import logging
import traceback
import json
from pymongo import MongoClient
from pymongo.errors import WriteConcernError, WriteError

#############################################
""" CRUD Functionality for AAC Database """
#############################################

class AnimalShelter:
    def __init__(self, username, password, auth_source):
        """
        Initialize AnimalShelter with MongoDB connection details.

        Args:
            username (str): MongoDB username.
            password (str): MongoDB password.
            auth_source (str): MongoDB authentication source.
        """
        self.client = MongoClient(username=username, password=password, authSource=auth_source)
        self.db = self.client['AAC']
        self.collection = self.db['Animals']
        self.logger = logging.getLogger(__name__)

    def create(self, doc_to_insert: dict) -> bool:
        """
        C in CRUD: Create a new document.

        Args:
            doc_to_insert (dict): Document to be inserted.

        Returns:
            bool: True if successful, False otherwise.
        """
        try:
            if not doc_to_insert:
                raise ValueError("Nothing to save, data parameter empty")

            self.collection.insert_one(doc_to_insert)
            self.logger.info("Document inserted successfully")
            return True
        except Exception as e:
            self.logger.error("Error occurred during document creation: %s", str(e))
            traceback.print_exc()
            return False

    def find_docs(self, search_criteria: dict):
        """
        R in CRUD: Find documents matching search criteria.

        Args:
            search_criteria (dict): Criteria to search for documents.

        Returns:
            list: List of matching documents.
        """
        try:
            if not search_criteria:
                raise ValueError("Invalid search criteria")

            results = self.collection.find(search_criteria)
            return list(results)
        except Exception as e:
            self.logger.error("Error occurred during document retrieval: %s", str(e))
            traceback.print_exc()
            return []

    def update(self, look_up_values: dict, new_values: dict):
        """
        U in CRUD: Update documents.

        Args:
            look_up_values (dict): Criteria to find documents to update.
            new_values (dict): New values to update in the matching documents.

        Returns:
            dict: Result of the update operation.
        """
        try:
            if not look_up_values or not new_values:
                raise ValueError("Invalid lookup or update values")

            result = self.collection.update_one(look_up_values, {"$set": new_values})
            if result.modified_count != 0:
                self.logger.info("Document updated successfully")
            else:
                self.logger.info("Failed to update document")
            return result.raw_result
        except WriteConcernError as wce:
            self.logger.error("Write concern error occurred during document update: %s", str(wce))
            return str(wce)
        except WriteError as we:
            self.logger.error("Write error occurred during document update: %s", str(we))
            return str(we)
        except Exception as e:
            self.logger.error("Error occurred during document update: %s", str(e))
            traceback.print_exc()
            return None

    def delete(self, doc_to_delete: dict):
        """
        D in CRUD: Delete documents.

        Args:
            doc_to_delete (dict): Document to be deleted.

        Returns:
            dict: Result of the delete operation.
        """
        try:
            if not doc_to_delete:
                raise ValueError("Invalid document to delete")

            result = self.collection.delete_one(doc_to_delete)
            if result.deleted_count > 0:
                self.logger.info("Deletion successful")
            else:
                self.logger.info("Failed to delete document")
            return result.raw_result
        except WriteConcernError as wce:
            self.logger.error("Write concern error occurred during document deletion: %s", str(wce))
            return str(wce)
        except WriteError as we:
            self.logger.error("Write error occurred during document deletion: %s", str(we))
            return str(we)
        except Exception as e:
            self.logger.error("Error occurred during document deletion: %s", str(e))
            traceback.print_exc()
            return None
