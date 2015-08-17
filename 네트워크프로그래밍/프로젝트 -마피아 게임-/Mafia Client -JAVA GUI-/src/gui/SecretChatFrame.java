package gui;

/**
 * 비밀 채팅창은 MainFrame을 상속받습니다.
 * @author kimchangyu
 *
 */
public class SecretChatFrame extends MainFrame {
	/**
	 * Create the frame.
	 */
	public SecretChatFrame(String userName, String IPAddress, int PortNumber, Log log) {
		super(userName,IPAddress, PortNumber ,log);
		//비밀 채팅방은 기본적으로 채팅 가능합니다.
		process.currentLevel = Level.DAY;
		super.setTitle("비밀 채팅창");
		super.btnSecretChat.setVisible(false);
		super.btnVote.setVisible(false);
		super.missionNameLabel.setVisible(false);
		process.missionUserName = "Fail";
	}
}
