import numpy as np
import pandas as pd
from sklearn.preprocessing import MinMaxScaler, StandardScaler

def prepare_data(training_data, new_data):

  # create a copy of the training_data
  data_copy = training_data.copy()

  # create a copy of new_data.
  modified_new_data = new_data.copy()

  # Create SpecialProperty
  modified_new_data['SpecialProperty'] = modified_new_data["blood_type"].isin(["O+", "B+"]).astype(int)

  # Create a new feature for each symptom
  symptoms_df = modified_new_data['symptoms'].str.get_dummies(sep=';')
  modified_new_data = pd.merge(modified_new_data, symptoms_df, left_index=True, right_index=True)

  # Numerize the Sex column
  modified_new_data['isMale'] = modified_new_data["sex"].isin(["M"]).astype(int)

  # Extract the x and y coordinates from the current_location column and integrate them as features into the training set.
  current_location_col = modified_new_data['current_location']
  current_location_col = pd.DataFrame(current_location_col.str.replace('(',''), columns = ['current_location'])
  current_location_col = pd.DataFrame(current_location_col.current_location.str.replace(')',''), columns = ['current_location'])
  current_location_col = pd.DataFrame(current_location_col.current_location.str.replace("'",''), columns = ['current_location'])
  current_location_col = current_location_col.fillna('0, 0')
  current_location_col = current_location_col['current_location'].str.split(',', expand=True)
  current_location_col.rename(columns={0:'current_location_x', 1:'current_location_y'}, inplace=True)
  current_location_col['current_location_x'] = current_location_col['current_location_x'].astype(float)
  current_location_col['current_location_y'] = current_location_col['current_location_y'].astype(float)

  modified_new_data = pd.merge(modified_new_data, current_location_col, left_index=True, right_index=True)

  # Extract the x and y coordinates from the current_location column and integrate them as features into the new_data set.
  current_location_col = data_copy['current_location']
  current_location_col = pd.DataFrame(current_location_col.str.replace('(',''), columns = ['current_location'])
  current_location_col = pd.DataFrame(current_location_col.current_location.str.replace(')',''), columns = ['current_location'])
  current_location_col = pd.DataFrame(current_location_col.current_location.str.replace("'",''), columns = ['current_location'])
  current_location_col = current_location_col.fillna('0, 0')
  current_location_col = current_location_col['current_location'].str.split(',', expand=True)
  current_location_col.rename(columns={0:'current_location_x', 1:'current_location_y'}, inplace=True)
  current_location_col['current_location_x'] = current_location_col['current_location_x'].astype(float)
  current_location_col['current_location_y'] = current_location_col['current_location_y'].astype(float)

  data_copy = pd.merge(data_copy, current_location_col, left_index=True, right_index=True)


  # Normalize all our features using one of the two scalers: MinMaxScaler or StandardScaler

  # Features to be standardized
  features_to_standardize = ['age', 'weight', 'current_location_x', 'current_location_y', 'num_of_siblings', 
                              'happiness_score', 'household_income', 'conversations_per_day', 'sugar_levels', 
                              'sport_activity', 'PCR_04', 'PCR_07', 'PCR_08', 'PCR_09', 'PCR_10']

  # Features to be min-max scaled
  features_to_minmax_scale = ['PCR_01', 'PCR_02', 'PCR_03', 'PCR_05', 'PCR_06']

  # StandardScaler for features to be standardized
  standard_scaler = StandardScaler()
  standard_scaler.fit(data_copy[features_to_standardize])
  modified_new_data[features_to_standardize] = standard_scaler.transform(modified_new_data[features_to_standardize])

  # MinMaxScaler for features to be min-max scaled
  minmax_scaler = MinMaxScaler()
  minmax_scaler.fit(data_copy[features_to_minmax_scale])
  modified_new_data[features_to_minmax_scale] = minmax_scaler.transform(modified_new_data[features_to_minmax_scale])

  # Drop the non-useful features
  modified_new_data = modified_new_data.drop(['patient_id', 'sex', 'blood_type', 'current_location', 'symptoms', 'pcr_date'], axis=1)

  return modified_new_data