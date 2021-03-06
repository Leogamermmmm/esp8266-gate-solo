import styled from 'styled-components/native';

export const ControlsList = styled.FlatList.attrs({
  showsVerticalScrollIndicator: false,
})``;

export const Control = styled.TouchableOpacity.attrs({
  activeOpacity: 0.8,
})`
  border-bottom-color: #4d4d4d;
  border-bottom-width: 1px;
  padding: 25px 20px;

  flex-direction: row;
  align-items: center;
`;

export const Name = styled.Text`
  font-size: 16px;
  color: #fff;
  margin: 0 35px 0 15px;
`;
