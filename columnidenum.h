#ifndef COLUMNIDENUM_H
#define COLUMNIDENUM_H

/*
 * This enum is for use in selecting column names to display and the order of
 * the comlumns in the table display.
 */
enum class ColumnIds
{
    NO_COLUMN,
    DATE,
    // Vital Signs
    TEMPERATURE,
    PUSLE_RATE,
    RESPIRATION_RATE,
    BLOOD_PRESSURE,
    BLOOD_OXYGEN,
    WEIGHT,
    BLOOD_SUGAR,
    // Other things that might affect vital signs
    EXERCISE,
    SLEEP_HOURS,
    SLEEP_INTERRUPTIONS,
    BOWEL_MOVEMENTS,
    // Nutritional Information
    NUTRITION_CALORIES,
    NUTRITION_SATURATED_FAT,
    NUTRITION_TRANS_FAT,
    NUTRITION_CHOLESTEROL,
    NUTRITION_SODIUM,
    NUTRITION_FIBER,
    NUTRITION_TOTAL_SUGARS,
    NUTRITION_PROTIEN,
    NUTRITION_VITAMIN_D,
    NUTRITION_CALCIUM,
    NUTRITION_IRON,
    NUTRITION_POTASSIUM,
    NOTES,
    LAST_COLUMN_ID
};

#endif // COLUMNIDENUM_H
