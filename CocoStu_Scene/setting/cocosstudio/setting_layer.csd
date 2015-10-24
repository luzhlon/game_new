<GameProjectFile>
  <PropertyGroup Type="Layer" Name="setting_layer" ID="3c9234e3-0b13-477f-a7dc-d691ae53176f" Version="2.3.2.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer1" Tag="20" ctype="GameLayerObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="-1083892462" Tag="2" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="image_bg" ActionTag="-1850219301" Tag="10" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Width="256" Scale9Height="78" ctype="ImageViewObjectData">
                <Size X="960.0000" Y="640.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="480.0000" Y="320.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="bg1.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="ScrollView" ActionTag="-1934234580" Tag="15" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
                <Size X="960.0000" Y="640.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_1" ActionTag="-295287808" Tag="16" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="-4.6080" BottomMargin="4.6080" TouchEnable="True" BackColorAlpha="0" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="960.0000" Y="640.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1" ActionTag="299725935" Tag="17" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="209.3040" RightMargin="585.6960" TopMargin="121.3320" BottomMargin="471.6680" FontSize="36" LabelText="服务器IP:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="165.0000" Y="47.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="374.3040" Y="495.1680" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="0" />
                        <PrePosition X="0.3899" Y="0.7737" />
                        <PreSize X="0.1406" Y="0.1111" />
                        <FontResource Type="Normal" Path="LiXuke_Art.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="text_ip" ActionTag="-575338691" Tag="18" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="472.9920" RightMargin="247.0080" TopMargin="129.2800" BottomMargin="462.7200" TouchEnable="True" FontSize="36" IsCustomSize="True" LabelText="192.168.191.2" PlaceHolderText="" MaxLengthEnable="True" MaxLengthText="16" ctype="TextFieldObjectData">
                        <Size X="240.0000" Y="48.0000" />
                        <AnchorPoint />
                        <Position X="472.9920" Y="462.7200" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4927" Y="0.7230" />
                        <PreSize X="0.2500" Y="0.0750" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="slider_volume" ActionTag="-1797868785" Tag="19" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="474.9120" RightMargin="245.0880" TopMargin="214.0160" BottomMargin="406.7840" TouchEnable="True" PercentInfo="67" ctype="SliderObjectData">
                        <Size X="240.0000" Y="19.2000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="594.9120" Y="416.3840" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="144" G="238" B="144" />
                        <PrePosition X="0.6197" Y="0.6506" />
                        <PreSize X="0.2500" Y="0.0300" />
                        <BackGroundData Type="Default" Path="Default/Slider_Back.png" Plist="" />
                        <ProgressBarData Type="Default" Path="Default/Slider_PressBar.png" Plist="" />
                        <BallNormalData Type="Default" Path="Default/SliderNode_Normal.png" Plist="" />
                        <BallPressedData Type="Default" Path="Default/SliderNode_Press.png" Plist="" />
                        <BallDisabledData Type="Default" Path="Default/SliderNode_Disable.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2" ActionTag="-1125783724" Tag="20" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="213.3040" RightMargin="585.6960" TopMargin="195.5720" BottomMargin="397.4280" FontSize="36" LabelText="游戏音量:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="47.0000" />
                        <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                        <Position X="374.3040" Y="420.9280" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="0" />
                        <PrePosition X="0.3899" Y="0.6577" />
                        <PreSize X="0.1406" Y="0.1111" />
                        <FontResource Type="Normal" Path="LiXuke_Art.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleY="1.0000" />
                    <Position Y="644.6080" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="1.0072" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="144" G="238" B="144" />
                <PrePosition />
                <PreSize X="1.0000" Y="1.0000" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="960" Height="640" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_ok" ActionTag="107854060" Tag="21" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="561.4560" RightMargin="228.5280" TopMargin="482.5600" BottomMargin="103.4240" TouchEnable="True" FontSize="36" ButtonText="确定" Scale9Width="170" Scale9Height="54" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="170.0160" Y="54.0160" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="646.4640" Y="130.4320" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6734" Y="0.2038" />
                <PreSize X="0.1771" Y="0.0844" />
                <FontResource Type="Normal" Path="LiXuke_Art.ttf" Plist="" />
                <TextColor A="255" R="30" G="144" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="btn3_press.png" Plist="" />
                <NormalFileData Type="Normal" Path="btn3_normal.png" Plist="" />
                <OutlineColor A="255" R="0" G="0" B="0" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_cancel" ActionTag="891252588" Tag="44" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="250.3200" RightMargin="539.6640" TopMargin="483.2640" BottomMargin="102.7200" TouchEnable="True" FontSize="36" ButtonText="取消" Scale9Width="170" Scale9Height="54" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="170.0160" Y="54.0160" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="335.3280" Y="129.7280" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3493" Y="0.2027" />
                <PreSize X="0.1771" Y="0.0844" />
                <FontResource Type="Normal" Path="LiXuke_Art.ttf" Plist="" />
                <TextColor A="255" R="255" G="0" B="0" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="btn3_press.png" Plist="" />
                <NormalFileData Type="Normal" Path="btn3_normal.png" Plist="" />
                <OutlineColor A="255" R="0" G="0" B="0" />
                <ShadowColor A="255" R="0" G="0" B="0" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>