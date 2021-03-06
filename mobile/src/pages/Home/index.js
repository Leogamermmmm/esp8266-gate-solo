import React, { useState, useEffect } from 'react';
import { TouchableOpacity } from 'react-native';

import { useNavigation } from '@react-navigation/native';
import { withNavigationFocus } from '@react-navigation/compat';

import { Feather, AntDesign } from '@expo/vector-icons';

import AsyncStorage from '@react-native-community/async-storage';

import PropTypes from 'prop-types';

import Container from '../../Components/Container';
import { Header, HeaderTitle } from '../../Components/Header';

import { ControlsList, Control, Name } from './styles';

function Home({ isFocused }) {
  const navigation = useNavigation();

  const [controls, setControls] = useState([]);

  async function getControls() {
    const response = await AsyncStorage.getItem('controls');

    if (response) {
      setControls(JSON.parse(response));
    }
  }

  useEffect(() => {
    if (isFocused) {
      getControls();
    }
  }, [isFocused]);

  return (
    <Container>
      <Header>
        <HeaderTitle>Gate Remote</HeaderTitle>

        <TouchableOpacity
          onPress={() => navigation.navigate('New')}
          activeOpacity={0.8}
        >
          <Feather name="plus" color="#fff" size={32} />
        </TouchableOpacity>
      </Header>

      <ControlsList
        data={controls}
        keyExtractor={(control) => String(control.id)}
        renderItem={({ item: control }) => (
          <Control
            onPress={() => navigation.navigate('Control', { control })}
            onLongPress={() => navigation.navigate('Edit', { control })}
          >
            <AntDesign name="home" color="#fff" size={36} />

            <Name numberOfLines={1}>{control.name}</Name>
          </Control>
        )}
      />
    </Container>
  );
}

Home.propTypes = {
  isFocused: PropTypes.bool.isRequired,
};

export default withNavigationFocus(Home);
